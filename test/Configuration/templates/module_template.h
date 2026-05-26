/**
 ******************************************************************************
 * @file    module_template.h
 * @brief   Module Template Header
 * @details Reusable template for all embedded system modules.
 *          Demonstrates proper conditional compilation pattern.
 *
 *          Usage:
 *          - Copy this file to your module's header file
 *          - Replace MODULE_NAME with your module name
 *          - Replace ENABLE_MODULE with your module's enable macro
 *          - Implement module-specific functionality
 *
 * @author  Your Name
 * @email   your.email@example.com
 * @date    YYYY-MM-DD
 ******************************************************************************
 */

#ifndef _MODULE_NAME_H
#define _MODULE_NAME_H

/* ======================================================================== */
/*                        (1) INCLUDE MASTER CONFIGURATION FILE FIRST       */
/* ======================================================================== */

/**
 * @brief Include master configuration file
 * @note Must be included first to access module enable/disable flags
 */
#include "app_modules_config.h"

/* ======================================================================== */
/*                        C++ COMPATIBILITY                                 */
/* ======================================================================== */

#if defined __cplusplus
extern "C" {
#endif

/* ======================================================================== */
/*                        (2) CONDITIONAL COMPILATION                           */
/* ======================================================================== */

/**
 * @brief Module enable/disable macro
 * @note Only compile this entire file if ENABLE_MODULE is enabled
 *       When disabled, stub functions are provided to prevent linking errors
 */
#if ENABLE_MODULE

/* ======================================================================== */
/*                        (3) SYSTEM INCLUDES                               */
/* ======================================================================== */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* Add additional system includes as needed */
/* #include <some_system_header.h> */

/* ======================================================================== */
/*                        (4) CONFIGURATION MACROS                          */
/* ======================================================================== */

/**
 * @brief Module configuration macros
 * @details These can be overridden from build system
 *          Provide sensible defaults that work for most applications
 */

/**
 * @brief Enable auto-initialization on startup
 * @note 1 = module initializes automatically on startup
 *       0 = application must call module_init() explicitly
 */
#ifndef MODULE_NAME_ENABLE_AUTO_INIT
    #define MODULE_NAME_ENABLE_AUTO_INIT 1
#endif

/**
 * @brief Module buffer size
 * @note Size of internal buffer (bytes)
 */
#ifndef MODULE_NAME_BUFFER_SIZE
    #define MODULE_NAME_BUFFER_SIZE  1024
#endif

/**
 * @brief Module timeout (milliseconds)
 * @note Default timeout for module operations
 */
#ifndef MODULE_NAME_TIMEOUT_MS
    #define MODULE_NAME_TIMEOUT_MS   1000
#endif

/**
 * @brief Maximum number of concurrent operations
 */
#ifndef MODULE_NAME_MAX_OPS
    #define MODULE_NAME_MAX_OPS      5
#endif

/* ======================================================================== */
/*                        (5) TYPE DEFINITIONS                             */
/* ======================================================================== */

/**
 * @brief Module error codes
 */
typedef enum {
    MODULE_NAME_OK           = 0,  /**< Operation successful */
    MODULE_NAME_ERR         = 1,  /**< Generic error */
    MODULE_NAME_ERR_INVAL   = 2,  /**< Invalid parameter */
    MODULE_NAME_ERR_NOMEM   = 3,  /**< Out of memory */
    MODULE_NAME_ERR_BUSY    = 4,  /**< Module busy */
    MODULE_NAME_ERR_TIMEOUT  = 5,  /**< Operation timeout */
    MODULE_NAME_ERR_IO      = 6,  /**< I/O error */
    MODULE_NAME_ERR_PERM    = 7,  /**< Permission denied */
    MODULE_NAME_ERR_NOTSUP  = 8,  /**< Operation not supported */
} module_name_err_t;

/**
 * @brief Module state enumeration
 */
typedef enum {
    MODULE_NAME_STATE_UNINITIALIZED = 0,  /**< Module not initialized */
    MODULE_NAME_STATE_INITIALIZED   = 1,  /**< Module initialized */
    MODULE_NAME_STATE_RUNNING      = 2,  /**< Module running */
    MODULE_NAME_STATE_ERROR        = 3,  /**< Module in error state */
} module_name_state_t;

/**
 * @brief Module configuration structure
 */
typedef struct {
    uint32_t buffer_size;         /**< Buffer size override */
    uint32_t timeout_ms;          /**< Timeout override (ms) */
    uint8_t  max_ops;            /**< Max concurrent operations */
    bool      auto_init;          /**< Auto-initialize flag */
} module_name_config_t;

/* ======================================================================== */
/*                        (6) PUBLIC API                                   */
/* ======================================================================== */

/**
 * @brief Initialize module with default configuration
 * @return MODULE_NAME_OK on success, error code otherwise
 *
 * @note This function initializes the module with default configuration.
 *       Must be called before any other module functions.
 */
module_name_err_t module_name_init(void);

/**
 * @brief Initialize module with custom configuration
 * @param config Pointer to configuration structure
 * @return MODULE_NAME_OK on success, error code otherwise
 *
 * @note Allows fine-tuning module behavior at runtime
 */
module_name_err_t module_name_init_config(const module_name_config_t *config);

/**
 * @brief Deinitialize module
 * @return MODULE_NAME_OK on success, error code otherwise
 *
 * @note Releases all resources allocated by the module
 */
module_name_err_t module_name_deinit(void);

/**
 * @brief Get current module state
 * @return Current module state
 */
module_name_state_t module_name_get_state(void);

/**
 * @brief Process module background tasks
 * @details Should be called periodically from main loop or task
 */
void module_name_process(void);

/**
 * @brief Reset module to default state
 * @return MODULE_NAME_OK on success, error code otherwise
 */
module_name_err_t module_name_reset(void);

/* ======================================================================== */
/*                        END OF CONDITIONAL COMPILATION                 */
/* ======================================================================== */

#else /* !ENABLE_MODULE */

/* ======================================================================== */
/*                        STUB FUNCTIONS FOR DISABLED MODULE            */
/* ======================================================================== */

/**
 * @brief Stub type definitions for disabled module
 */
typedef enum {
    MODULE_NAME_OK       = 0,
    MODULE_NAME_ERR     = 1,
} module_name_err_t;

typedef enum {
    MODULE_NAME_STATE_UNINITIALIZED = 0,
} module_name_state_t;

typedef struct {
    int dummy; /**< Unused placeholder */
} module_name_config_t;

/**
 * @brief Stub initialization function
 * @return MODULE_NAME_ERR to indicate module is not available
 */
static inline module_name_err_t module_name_init(void)
{
    return MODULE_NAME_ERR;
}

/**
 * @brief Stub initialization function with config
 * @return MODULE_NAME_ERR to indicate module is not available
 */
static inline module_name_err_t module_name_init_config(const module_name_config_t *config)
{
    (void)config;
    return MODULE_NAME_ERR;
}

/**
 * @brief Stub deinitialization function
 * @return MODULE_NAME_ERR to indicate module is not available
 */
static inline module_name_err_t module_name_deinit(void)
{
    return MODULE_NAME_ERR;
}

/**
 * @brief Stub state function
 * @return MODULE_NAME_STATE_UNINITIALIZED
 */
static inline module_name_state_t module_name_get_state(void)
{
    return MODULE_NAME_STATE_UNINITIALIZED;
}

/**
 * @brief Stub process function
 * @details No operation when module is disabled
 */
static inline void module_name_process(void)
{
    /* No operation */
}

/**
 * @brief Stub reset function
 * @return MODULE_NAME_ERR to indicate module is not available
 */
static inline module_name_err_t module_name_reset(void)
{
    return MODULE_NAME_ERR;
}

#endif /* ENABLE_MODULE */

/* ======================================================================== */
/*                        C++ COMPATIBILITY                                 */
/* ======================================================================== */

#if defined __cplusplus
}
#endif

/* ======================================================================== */
/*                        END OF FILE                                      */
/* ======================================================================== */

#endif /* _MODULE_NAME_H */
