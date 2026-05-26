/**
 ******************************************************************************
 * @file    module_template.c
 * @brief   Module Template Implementation
 * @details Reusable template for all embedded system modules.
 *          Demonstrates proper conditional compilation pattern.
 *
 *          Usage:
 *          - Copy this file to your module's source file
 *          - Replace MODULE_NAME with your module name
 *          - Replace ENABLE_MODULE with your module's enable macro
 *          - Implement module-specific functionality
 ******************************************************************************
 * @author  Your Name
 * @email   your.email@example.com
 * @date    YYYY-MM-DD
 ******************************************************************************
 */

/* ======================================================================== */
/*                        (1) INCLUDE MASTER CONFIGURATION FILE FIRST       */
/* ======================================================================== */

#include "app_modules_config.h"

/* ======================================================================== */
/*                        (2) CONDITIONAL COMPILATION                           */
/* ======================================================================== */

/**
 * @brief Module enable/disable macro
 * @note Only compile this entire file if ENABLE_MODULE is enabled
 *       When disabled, this file generates no code
 */
#if ENABLE_MODULE

/* ======================================================================== */
/*                        (3) SYSTEM / 3RD-PARTY INCLUDES ----------------- */
/* ======================================================================== */

#include <string.h>
#include <stdlib.h>

#include <kservice.h>

#include "module_template.h"

/* ======================================================================== */
/*                        (4) LOCAL DEFINITIONS                             */
/* ======================================================================== */

/**
 * @brief Module name for logging
 */
#define __THIS_MODULE_NAME__   "module_template"

/**
 * @brief Module version
 */
#define MODULE_NAME_VERSION     "1.0.0"

/* ======================================================================== */
/*                        (5) PRIVATE TYPEDEF                              */
/* ======================================================================== */

/**
 * @brief Module context structure
 */
typedef struct {
    module_name_state_t state;          /**< Current module state */
    module_name_config_t config;       /**< Module configuration */
    uint8_t *buffer;                  /**< Internal buffer */
    uint32_t buffer_size;             /**< Actual buffer size */
    uint32_t init_count;              /**< Initialization counter */
} module_name_context_t;

/* ======================================================================== */
/*                        (6) PRIVATE DATA                                 */
/* ======================================================================== */

/**
 * @brief Static module context
 */
static module_name_context_t g_module_ctx = {
    .state = MODULE_NAME_STATE_UNINITIALIZED,
    .buffer = NULL,
    .buffer_size = 0,
    .init_count = 0,
};

/* ======================================================================== */
/*                        (7) PRIVATE FUNCTION DECLARATIONS ----------------- */
/* ======================================================================== */

/**
 * @brief Validate configuration
 * @param config Pointer to configuration structure
 * @return true if valid, false otherwise
 */
static bool validate_config(const module_name_config_t *config);

/**
 * @brief Allocate internal buffer
 * @param size Requested buffer size
 * @return MODULE_NAME_OK on success, error code otherwise
 */
static module_name_err_t allocate_buffer(uint32_t size);

/**
 * @brief Free internal buffer
 */
static void free_buffer(void);

/* ======================================================================== */
/*                        PRIVATE FUNCTION IMPLEMENTATIONS -------------- */
/* ======================================================================== */

/**
 * @brief Validate configuration
 */
static bool validate_config(const module_name_config_t *config)
{
    if (config == NULL) {
        return false;
    }

    /* Validate buffer size */
    if (config->buffer_size == 0 || config->buffer_size > 65536) {
        return false;
    }

    /* Validate timeout */
    if (config->timeout_ms == 0 || config->timeout_ms > 60000) {
        return false;
    }

    /* Validate max operations */
    if (config->max_ops == 0 || config->max_ops > 255) {
        return false;
    }

    return true;
}

/**
 * @brief Allocate internal buffer
 */
static module_name_err_t allocate_buffer(uint32_t size)
{
    /* Free existing buffer if any */
    free_buffer();

    /* Allocate new buffer */
    g_module_ctx.buffer = (uint8_t *)csp_calloc(1, size);
    if (g_module_ctx.buffer == NULL) {
        csp_log_error("%s: Failed to allocate buffer (%u bytes)",
                      __THIS_MODULE_NAME__, size);
        return MODULE_NAME_ERR_NOMEM;
    }

    g_module_ctx.buffer_size = size;
    csp_log_info("%s: Allocated buffer (%u bytes)",
                 __THIS_MODULE_NAME__, size);

    return MODULE_NAME_OK;
}

/**
 * @brief Free internal buffer
 */
static void free_buffer(void)
{
    if (g_module_ctx.buffer != NULL) {
        csp_free(g_module_ctx.buffer);
        g_module_ctx.buffer = NULL;
        g_module_ctx.buffer_size = 0;
        csp_log_debug("%s: Freed internal buffer", __THIS_MODULE_NAME__);
    }
}

/* ======================================================================== */
/*                        PUBLIC API IMPLEMENTATION ---------------------- */
/* ======================================================================== */

/**
 * @brief Initialize module with default configuration
 */
module_name_err_t module_name_init(void)
{
    /* Use default configuration */
    module_name_config_t default_config = {
        .buffer_size = MODULE_NAME_BUFFER_SIZE,
        .timeout_ms  = MODULE_NAME_TIMEOUT_MS,
        .max_ops     = MODULE_NAME_MAX_OPS,
        .auto_init   = MODULE_NAME_ENABLE_AUTO_INIT,
    };

    return module_name_init_config(&default_config);
}

/**
 * @brief Initialize module with custom configuration
 */
module_name_err_t module_name_init_config(const module_name_config_t *config)
{
    /* Check if already initialized */
    if (g_module_ctx.state == MODULE_NAME_STATE_INITIALIZED ||
        g_module_ctx.state == MODULE_NAME_STATE_RUNNING) {
        csp_log_warn("%s: Module already initialized (state=%d)",
                     __THIS_MODULE_NAME__, g_module_ctx.state);
        return MODULE_NAME_ERR_BUSY;
    }

    /* Validate configuration */
    if (!validate_config(config)) {
        csp_log_error("%s: Invalid configuration", __THIS_MODULE_NAME__);
        return MODULE_NAME_ERR_INVAL;
    }

    /* Store configuration */
    g_module_ctx.config = *config;
    csp_log_info("%s: Initializing module v%s (buffer=%u, timeout=%u, ops=%u)",
                 __THIS_MODULE_NAME__, MODULE_NAME_VERSION,
                 config->buffer_size, config->timeout_ms, config->max_ops);

    /* Allocate buffer */
    module_name_err_t err = allocate_buffer(config->buffer_size);
    if (err != MODULE_NAME_OK) {
        return err;
    }

    /* Perform module-specific initialization here */
    /* TODO: Add your module-specific initialization code */

    /* Update state */
    g_module_ctx.state = MODULE_NAME_STATE_INITIALIZED;
    g_module_ctx.init_count++;

    csp_log_info("%s: Module initialized successfully (init_count=%u)",
                 __THIS_MODULE_NAME__, g_module_ctx.init_count);

    return MODULE_NAME_OK;
}

/**
 * @brief Deinitialize module
 */
module_name_err_t module_name_deinit(void)
{
    /* Check if initialized */
    if (g_module_ctx.state == MODULE_NAME_STATE_UNINITIALIZED) {
        csp_log_warn("%s: Module not initialized", __THIS_MODULE_NAME__);
        return MODULE_NAME_OK;
    }

    csp_log_info("%s: Deinitializing module", __THIS_MODULE_NAME__);

    /* Perform module-specific deinitialization here */
    /* TODO: Add your module-specific deinitialization code */

    /* Free resources */
    free_buffer();

    /* Reset state */
    g_module_ctx.state = MODULE_NAME_STATE_UNINITIALIZED;

    csp_log_info("%s: Module deinitialized successfully", __THIS_MODULE_NAME__);

    return MODULE_NAME_OK;
}

/**
 * @brief Get current module state
 */
module_name_state_t module_name_get_state(void)
{
    return g_module_ctx.state;
}

/**
 * @brief Process module background tasks
 */
void module_name_process(void)
{
    /* Check if initialized */
    if (g_module_ctx.state != MODULE_NAME_STATE_INITIALIZED &&
        g_module_ctx.state != MODULE_NAME_STATE_RUNNING) {
        return;
    }

    /* Process background tasks here */
    /* TODO: Add your module-specific background processing */

    /* Kick watchdog */
    thread_wd_kick_self();
}

/**
 * @brief Reset module to default state
 */
module_name_err_t module_name_reset(void)
{
    csp_log_info("%s: Resetting module", __THIS_MODULE_NAME__);

    /* Deinitialize if initialized */
    if (g_module_ctx.state != MODULE_NAME_STATE_UNINITIALIZED) {
        module_name_err_t err = module_name_deinit();
        if (err != MODULE_NAME_OK) {
            csp_log_error("%s: Failed to deinitialize during reset (err=%d)",
                          __THIS_MODULE_NAME__, err);
            return err;
        }
    }

    /* Reinitialize with default config */
    return module_name_init();
}

/* ======================================================================== */
/*                        SHELL COMMANDS (Optional) ----------------------- */
/* ======================================================================== */

#ifdef TSC_USING_FINSH

/**
 * @brief Shell command to initialize module
 * Usage: module_init
 */
static void __module_init_cmd(int argc, char **argv)
{
    (void)argc;
    (void)argv;

    module_name_err_t err = module_name_init();
    if (err == MODULE_NAME_OK) {
        tsc_kprintf("Module initialized successfully\r\n");
    } else {
        tsc_kprintf("Failed to initialize module (err=%d)\r\n", err);
    }
}

/**
 * @brief Shell command to deinitialize module
 * Usage: module_deinit
 */
static void __module_deinit_cmd(int argc, char **argv)
{
    (void)argc;
    (void)argv;

    module_name_err_t err = module_name_deinit();
    if (err == MODULE_NAME_OK) {
        tsc_kprintf("Module deinitialized successfully\r\n");
    } else {
        tsc_kprintf("Failed to deinitialize module (err=%d)\r\n", err);
    }
}

/**
 * @brief Shell command to get module state
 * Usage: module_state
 */
static void __module_state_cmd(int argc, char **argv)
{
    (void)argc;
    (void)argv;

    module_name_state_t state = module_name_get_state();
    const char *state_str = "UNKNOWN";

    switch (state) {
        case MODULE_NAME_STATE_UNINITIALIZED:
            state_str = "UNINITIALIZED";
            break;
        case MODULE_NAME_STATE_INITIALIZED:
            state_str = "INITIALIZED";
            break;
        case MODULE_NAME_STATE_RUNNING:
            state_str = "RUNNING";
            break;
        case MODULE_NAME_STATE_ERROR:
            state_str = "ERROR";
            break;
    }

    tsc_kprintf("Module state: %s\r\n", state_str);
}

/**
 * @brief Shell command to reset module
 * Usage: module_reset
 */
static void __module_reset_cmd(int argc, char **argv)
{
    (void)argc;
    (void)argv;

    module_name_err_t err = module_name_reset();
    if (err == MODULE_NAME_OK) {
        tsc_kprintf("Module reset successfully\r\n");
    } else {
        tsc_kprintf("Failed to reset module (err=%d)\r\n", err);
    }
}

/* Command Registration */
TSC_CMD_EXPORT(__module_init_cmd, "Initialize module");
TSC_CMD_EXPORT(__module_deinit_cmd, "Deinitialize module");
TSC_CMD_EXPORT(__module_state_cmd, "Get module state");
TSC_CMD_EXPORT(__module_reset_cmd, "Reset module");

#endif /* TSC_USING_FINSH */

/* ======================================================================== */
/*                        END OF CONDITIONAL COMPILATION                 */
/* ======================================================================== */

/* No code is generated when ENABLE_MODULE is disabled */

#endif /* ENABLE_MODULE */
