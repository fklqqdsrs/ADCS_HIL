# CSP Enable Register Service

## Overview

The `CSP_ENABLE_REGISTER` macro provides a simple way to enable or disable the CSP Register Service in your application. This allows you to conditionally compile register functionality based on your system requirements.

## How to Use

### Enabling the Register Service (Default)

To enable the CSP Register Service, ensure `CSP_ENABLE_REGISTER` is defined to `1`:

```c
// In register_protocol.h or your project configuration
#define CSP_ENABLE_REGISTER      1
```

When enabled, the following features are available:
- Register service initialization via `register_service_init()`
- CSP thread for handling register requests
- CLI command `reg_server` for starting the service
- Full register read/write/add/remove operations

### Disabling the Register Service

To disable the CSP Register Service, set `CSP_ENABLE_REGISTER` to `0`:

```c
// In register_protocol.h or your project configuration
#define CSP_ENABLE_REGISTER      0
```

When disabled:
- Register service code is not compiled into the firmware
- `register_service_init()` becomes a stub function that returns immediately
- No memory or CPU resources are allocated for register operations
- CLI command still exists but logs a message indicating the service is disabled

## Configuration

### Default Configuration

The default value is set in `Inc/register_protocol.h`:

```c
#ifndef CSP_ENABLE_REGISTER
#define CSP_ENABLE_REGISTER      1   /*!< @brief Enable/Disable CSP Register Service @note Provides register access over CSP */
#endif
```

### Override in Project Configuration

You can override this value in your project's configuration file or compiler flags:

**Method 1: Define in project header**
```c
// In your project's config.h
#define CSP_ENABLE_REGISTER      0
```

**Method 2: Define in compiler flags**
```
-DCSP_ENABLE_REGISTER=0
```

**Method 3: Define in Makefile**
```makefile
CFLAGS += -DCSP_ENABLE_REGISTER=0
```

## Implementation Details

### Conditional Compilation

The implementation uses preprocessor directives to conditionally include or exclude code:

**In `Src/register_service_init.c`:**
```c
#include "register_protocol.h"

#if CSP_ENABLE_REGISTER
#include "csp_register_service.h"  // Only included if enabled
#include "register_management.h"   // Only included if enabled
#endif

#if CSP_ENABLE_REGISTER
int register_service_init(uint8_t server_port) {
    // Full implementation
}
#endif
```

### Stub Function

When disabled, a stub function ensures the API remains compatible:

```c
#if !CSP_ENABLE_REGISTER
int register_service_init(uint8_t server_port) {
    csp_log_info("Register Service is disabled (CSP_ENABLE_REGISTER=0)");
    return CSP_ERR_NONE;
}
#endif
```

This allows calling code to remain unchanged whether the service is enabled or disabled.

## Benefits

1. **Memory Savings**: Disabled service reduces flash and RAM usage
2. **Reduced Complexity**: Smaller codebase when register functionality is not needed
3. **Flexibility**: Easy to enable/disable without modifying application code
4. **Maintainability**: Single point of configuration for the entire register service

## Example Usage Scenarios

### Scenario 1: Development Phase
Enable register service for debugging and testing:
```c
#define CSP_ENABLE_REGISTER      1
```

### Scenario 2: Production Release
Disable register service to reduce attack surface and save memory:
```c
#define CSP_ENABLE_REGISTER      0
```

### Scenario 3: Different Product Variants
Use different configurations for different product variants:
- Variant A (with remote monitoring): `CSP_ENABLE_REGISTER 1`
- Variant B (standalone): `CSP_ENABLE_REGISTER 0`

## Testing

When toggling `CSP_ENABLE_REGISTER`, ensure you:
1. Rebuild the entire project
2. Verify register service behavior matches expected state
3. Test CLI commands if used
4. Check memory usage reports to confirm savings when disabled

## Files Modified

- `Inc/register_protocol.h` - Defines the `CSP_ENABLE_REGISTER` macro
- `Src/register_service_init.c` - Implements conditional compilation based on the macro

## Additional Notes

- The default value is `1` (enabled) to maintain backward compatibility
- Changes to this macro require a full project rebuild
- The CLI command `reg_server` remains available in both modes for easier debugging
- All register-related thread registrations are also conditionally compiled
