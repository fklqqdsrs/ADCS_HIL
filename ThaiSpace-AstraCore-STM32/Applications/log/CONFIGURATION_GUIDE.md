# CSP Log Configuration Guide

## Overview
This document describes the conditional compilation directives added to enable/disable logging and file backend functionality.

## Configuration Macros

### 1. CSP_ENABLE_LOG
**Purpose:** Enable or disable the entire logging system

**Usage:**
```c
// In your configuration header (e.g., hw_config.h or similar)
#define CSP_ENABLE_LOG  1   // Enable logging
// #define CSP_ENABLE_LOG  0   // Disable logging
```

**Files affected:**
- `Inc/tsc_log_core.h` - Main logging header
- `Src/tsc_log_core.c` - Core logging implementation
- `Src/console_be.c` - Console backend
- `Src/csp_log_server.c` - CSP log server
- `Src/csp_log_client.c` - CSP log client

**Effect when disabled:**
- All logging functionality is excluded from compilation
- Reduces code size and memory usage
- No logging overhead when not needed

### 2. CSP_ENABLE_FILE
**Purpose:** Enable or disable file backend for logging

**Usage:**
```c
// In your configuration header (e.g., hw_config.h or similar)
#define CSP_ENABLE_FILE  1   // Enable file backend
// #define CSP_ENABLE_FILE  0   // Disable file backend
```

**Files affected:**
- `Src/file_be.c` - File backend implementation

**Effect when disabled:**
- File logging backend is excluded from compilation
- Console and other backends remain functional (if CSP_ENABLE_LOG is enabled)
- Reduces memory and code size related to file operations

## Configuration Examples

### Example 1: Full Logging Enabled
```c
#define CSP_ENABLE_LOG  1
#define CSP_ENABLE_FILE  1
```
- All logging functionality enabled
- Console and file backends available
- Maximum debugging capability

### Example 2: Console Logging Only
```c
#define CSP_ENABLE_LOG  1
#define CSP_ENABLE_FILE  0
```
- Logging system enabled
- Only console backend available
- No file operations

### Example 3: Logging Disabled
```c
#define CSP_ENABLE_LOG  0
// CSP_ENABLE_FILE is irrelevant when CSP_ENABLE_LOG is 0
```
- All logging functionality excluded
- Minimal code size
- No logging overhead

## Implementation Details

### File Structure
```
#if CSP_ENABLE_LOG
    // All logging code here
#endif /* CSP_ENABLE_LOG */

#if CSP_ENABLE_FILE
    // File backend code here
#endif /* CSP_ENABLE_FILE */
```

### Dependencies
- `CSP_ENABLE_FILE` has no direct dependency on `CSP_ENABLE_LOG`
- However, file backend requires logging system to be useful
- It's recommended to enable `CSP_ENABLE_LOG` when using `CSP_ENABLE_FILE`

## Backends Overview

### Console Backend (`console_be.c`)
- Always available when `CSP_ENABLE_LOG` is enabled
- Controlled by `TSC_LOG_BACKEND_USING_CONSOLE` macro
- Outputs to hardware console

### File Backend (`file_be.c`)
- Available only when both `CSP_ENABLE_LOG` and `CSP_ENABLE_FILE` are enabled
- Uses FatFS for file operations
- Supports log rotation
- Configurable via:
  - `TSC_LOG_FILE_ROOT_PATH` - Log directory
  - `TSC_LOG_FILE_NAME_BASE` - Log filename
  - `TSC_LOG_FILE_MAX_NUM` - Maximum log files
  - `TSC_LOG_FILE_MAX_SIZE` - Maximum file size

### CSP Log Server/Client
- Available when `CSP_ENABLE_LOG` is enabled
- Network-based logging via CSP protocol
- Remote log filtering and control

## Memory Considerations

### When Logging is Disabled (`CSP_ENABLE_LOG = 0`)
- No log buffers allocated
- No backend structures
- No filter data structures
- Minimal code size impact

### When File Backend is Disabled (`CSP_ENABLE_FILE = 0`)
- No file handles or buffers
- No FatFS dependency
- Reduced stack usage

## Build Configuration

These macros should be defined in your project's configuration header file, typically located at:
- `Inc/hw_config.h`
- Or passed via compiler flags: `-DCSP_ENABLE_LOG=1 -DCSP_ENABLE_FILE=1`

## Verification

To verify the configuration is working correctly:

1. Check that the correct files are included in your build
2. Monitor code size changes when toggling macros
3. Test logging functionality when enabled
4. Ensure no compilation errors when disabled

## Notes

- Both macros default to undefined (treated as 0) if not set
- It's good practice to explicitly define them with 0 or 1
- Changes require recompilation of the entire project
- Header guard structure ensures proper inclusion order

## Version History

- v1.0 (2025) - Initial implementation of conditional compilation directives
