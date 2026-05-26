/**
 ******************************************************************************
 * @file    csp_app_params.h
 * @brief   CSP Application-Specific Parameters
 * @details This file contains application-specific CSP parameters that can
 *          be configured for your specific application needs. These are
 *          separate from the library configuration (csp_library_config.h)
 *          and module enable/disable settings (app_modules_config.h).
 *
 *          Use this file for:
 *          - Application-specific port assignments
 *          - Custom timeout values
 *          - Application-level buffer sizes
 *          - Protocol parameters specific to your application
 ******************************************************************************
 */

#ifndef CSP_APP_PARAMS_H
#define CSP_APP_PARAMS_H

/* ======================================================================== */
/*                        APPLICATION-SPECIFIC PORTS                       */
/* ======================================================================== */

/**
 * @brief Custom application ports
 * @note These are in addition to the standard CSP service ports defined
 *       in csp_library_config.h
 */
#define CSP_APP_CUSTOM_PORT_1    100
#define CSP_APP_CUSTOM_PORT_2    102
#define CSP_APP_CUSTOM_PORT_3    103
/* ======================================================================== */
/*                        APPLICATION TIMEOUTS                            */
/* ======================================================================== */

/**
 * @brief Application-specific timeout values (milliseconds)
 */
/* ======================================================================== */
/*                        APPLICATION BUFFER SIZES                        */
/* ======================================================================== */

/**
 * @brief Application buffer configurations
 */
/* ======================================================================== */
/*                        APPLICATION CONNECTION SETTINGS                 */
/* ======================================================================== */

/**
 * @brief Application connection parameters
 */
/* ======================================================================== */
/*                        APPLICATION RETRY SETTINGS                       */
/* ======================================================================== */

/**
 * @brief Application retry configuration
 */
#define CSP_APP_MAX_RETRIES      3
#define CSP_APP_RETRY_BACKOFF_MULTIPLIER 2

/* ======================================================================== */
/*                        NODE ADDRESSING                                 */
/* ======================================================================== */

/**
 * @brief Node addressing configuration
 * @note These can be overridden at runtime if needed
 */
#define CSP_APP_DEFAULT_NODE_ADDR 1
#define CSP_APP_SUBNET_ADDR      0
#define CSP_APP_DEFAULT_ROUTE_ADDR 0

/* ======================================================================== */
/*                        APPLICATION QUEUES                             */
/* ======================================================================== */

/**
 * @brief Application queue configurations
 */
#define CSP_APP_EVENT_QUEUE_SIZE 20
#define CSP_APP_CMD_QUEUE_SIZE   10

/* ======================================================================== */
/*                        APPLICATION FEATURE FLAGS                       */
/* ======================================================================== */

/**
 * @brief Application-specific feature flags
 * @note These are behavioral flags, not module enable/disable
 */
#define CSP_APP_ENABLE_LOOPBACK_TEST 0
#define CSP_APP_ENABLE_STATISTICS 1
#define CSP_APP_ENABLE_DIAGNOSTICS 1

/* ======================================================================== */
/*                        APPLICATION LOGGING                             */
/* ======================================================================== */

/**
 * @brief Application logging configuration
 */
#define CSP_APP_LOG_MAX_ENTRIES  100
#define CSP_APP_LOG_BUFFER_SIZE  4096

#endif /* CSP_APP_PARAMS_H */
