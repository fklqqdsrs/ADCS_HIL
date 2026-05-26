/**
 ******************************************************************************
 * @file    csp_route_server_config.h
 * @brief   CSP Route Server Configuration
 * @details Provides configurable feature switches and parameters for the
 *          CSP route server. Enable/disable features by setting these defines.
 ******************************************************************************
 * @author  Chaturong khachaban
 ******************************************************************************
 */

#ifndef __CSP_ROUTE_SERVER_CONFIG_H
#define __CSP_ROUTE_SERVER_CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif

#include <csp/interfaces/csp_if_can.h> // For CAN interface configuration
#include <csp/interfaces/csp_if_kiss.h> // For KISS interface configuration
#include <csp/interfaces/csp_if_i2c.h> // For I2C interface configuration

/* ======================================================================== */
/*                        FEATURE ENABLE/DISABLE SWITCHES                  */
/* ======================================================================== */

/**
 * @defgroup FEATURE_SWITCHES Feature Enable/Disable Switches
 * @brief Control which features are compiled into the code
 * @{
 */

/**
 * @brief Enable route table functionality
 * @details When enabled, allows dynamic route table configuration and loading
 * @note Set to 0 to disable route table, 1 to enable
 */
#ifndef CSP_ROUTE_SERVER_ENABLE_RTABLE
    #define CSP_ROUTE_SERVER_ENABLE_RTABLE          1
#endif

/**
 * @brief Enable default route table loading
 * @details When enabled, loads a default route table at initialization
 * @note Set to 0 to disable default route, 1 to enable
 */
#ifndef CSP_ROUTE_SERVER_ENABLE_DEFAULT_ROUTE
    #define CSP_ROUTE_SERVER_ENABLE_DEFAULT_ROUTE    1
#endif

/**
 * @brief Default route table string
 * @details Only used if CSP_ROUTE_SERVER_ENABLE_DEFAULT_ROUTE is enabled
 * @note Format: "dest/mask interface" e.g., "0/0 CAN" routes everything to CAN
 *       You can override this in your project's config file
 */
#ifndef CSP_ROUTE_SERVER_DEFAULT_ROUTE_TABLE
    #define CSP_ROUTE_SERVER_DEFAULT_ROUTE_TABLE     "0/0 CAN"
#endif

/**
 * @brief Enable debug information printing at initialization
 * @details When enabled, prints connection table, interfaces, and route table
 * @note Set to 0 to disable debug prints, 1 to enable
 */
#ifndef CSP_ROUTE_SERVER_ENABLE_DEBUG_PRINTS
    #define CSP_ROUTE_SERVER_ENABLE_DEBUG_PRINTS     1
#endif

/**
 * @brief Enable promiscuous mode for CAN interface
 * @details When enabled, CAN interface operates in promiscuous mode
 * @note Set to 0 to disable promiscuous mode, 1 to enable
 */
#ifndef CSP_ROUTE_SERVER_ENABLE_PROMISC_MODE
    #define CSP_ROUTE_SERVER_ENABLE_PROMISC_MODE     0
#endif

/**
 * @brief Enable CSP shell commands
 * @details When enabled, registers csp_send and csp_read commands to CLI
 * @note Set to 0 to disable shell commands, 1 to enable
 */
#ifndef CSP_ROUTE_SERVER_ENABLE_SHELL_CMDS
    #define CSP_ROUTE_SERVER_ENABLE_SHELL_CMDS       1
#endif

/**
 * @brief Enable KISS over UART interface
 * @details When enabled, KISS interface will be added
 * @note Only effective if ENABLE_HW_UART is defined
 */
#ifndef CSP_ROUTE_SERVER_ENABLE_KISS_IFACE
    #define CSP_ROUTE_SERVER_ENABLE_KISS_IFACE       1
#endif

/**
 * @brief Enable CAN interface
 * @details When enabled, CAN interface will be added
 * @note Only effective if ENABLE_HW_CAN is defined
 */
#ifndef CSP_ROUTE_SERVER_ENABLE_CAN_IFACE
    #define CSP_ROUTE_SERVER_ENABLE_CAN_IFACE        1
#endif

/**
 * @brief Enable I2C interface
 * @details When enabled, I2C interface will be added
 * @note Only effective if ENABLE_HW_I2C is defined
 */
#ifndef CSP_ROUTE_SERVER_ENABLE_I2C_IFACE
    #define CSP_ROUTE_SERVER_ENABLE_I2C_IFACE         1
#endif

/**
 * @brief Enable configuration system integration for route table
 * @details When enabled, exposes route_table to configuration system
 * @note Set to 0 to disable config integration, 1 to enable
 */
#ifndef CSP_ROUTE_SERVER_ENABLE_CONFIG_INTEGRATION
    #define CSP_ROUTE_SERVER_ENABLE_CONFIG_INTEGRATION  1
#endif

/** @} */ // End of Feature Switches

/* ======================================================================== */
/*                        ROUTE TABLE CONFIGURATION                        */
/* ======================================================================== */

/**
 * @defgroup ROUTE_TABLE_CONFIG Route Table Configuration
 * @brief Parameters related to route table functionality
 * @{
 */

#if CSP_ROUTE_SERVER_ENABLE_RTABLE
/**
 * @brief Route table buffer size
 * @details Size of buffer for storing route table configuration string
 * @note Increase if longer route table strings are needed
 */
#ifndef CSP_ROUTE_SERVER_RTABLE_BUFFER_SIZE
    #define CSP_ROUTE_SERVER_RTABLE_BUFFER_SIZE     256
#endif

/**
 * @brief Route table config variable name
 * @details Name exposed to configuration system
 */
#ifndef CSP_ROUTE_SERVER_RTABLE_CONFIG_NAME
    #define CSP_ROUTE_SERVER_RTABLE_CONFIG_NAME     "route_table"
#endif
#endif /* CSP_ROUTE_SERVER_ENABLE_RTABLE */

/** @} */ // End of Route Table Configuration

/* ======================================================================== */
/*                        CSP CONFIGURATION PARAMETERS                     */
/* ======================================================================== */

/**
 * @defgroup CSP_CONFIG_PARAMETERS CSP Configuration Parameters
 * @brief Basic CSP library configuration parameters
 * @{
 */

/**
 * @brief CSP node address
 * @details The address of this CSP node
 * @note Must be unique in the CSP network
 */
#ifndef CSP_ROUTE_SERVER_NODE_ADDRESS
    #define CSP_ROUTE_SERVER_NODE_ADDRESS           11
#endif

/**
 * @brief Maximum number of simultaneous CSP connections
 * @details Maximum concurrent connections this node can handle
 * @note Increase if expecting many simultaneous connections
 */
#ifndef CSP_ROUTE_SERVER_MAX_CONNECTIONS
    #define CSP_ROUTE_SERVER_MAX_CONNECTIONS        30
#endif

/**
 * @brief Number of CSP buffers
 * @details Number of packet buffers available in the pool
 * @note Increase if experiencing buffer allocation failures
 */
#ifndef CSP_ROUTE_SERVER_NUM_BUFFERS
    #define CSP_ROUTE_SERVER_NUM_BUFFERS             20
#endif

/**
 * @brief Routing task stack size
 * @details Stack size (in bytes) for CSP routing task
 * @note Adjust if routing task experiences stack overflow
 */
#ifndef CSP_ROUTE_SERVER_ROUTING_TASK_STACK
    #define CSP_ROUTE_SERVER_ROUTING_TASK_STACK     1024
#endif

/**
 * @brief Routing task priority
 * @details Priority level for CSP routing task
 * @note Use osPriority constants (e.g., osPriorityHigh, osPriorityNormal)
 */
#ifndef CSP_ROUTE_SERVER_ROUTING_TASK_PRIORITY
    #define CSP_ROUTE_SERVER_ROUTING_TASK_PRIORITY  osPriorityHigh
#endif

/**
 * @brief Server task timeout
 * @details Timeout (in milliseconds) for accepting connections
 * @note Adjust based on expected network latency
 */
#ifndef CSP_ROUTE_SERVER_ACCEPT_TIMEOUT_MS
    #define CSP_ROUTE_SERVER_ACCEPT_TIMEOUT_MS      10000
#endif

/**
 * @brief Packet read timeout
 * @details Timeout (in milliseconds) for reading packets from connection
 * @note Adjust based on expected network latency
 */
#ifndef CSP_ROUTE_SERVER_READ_TIMEOUT_MS
    #define CSP_ROUTE_SERVER_READ_TIMEOUT_MS        50
#endif

/**
 * @brief Connection backlog
 * @details Maximum number of pending connections in listen queue
 * @note Increase if expecting many simultaneous connection attempts
 */
#ifndef CSP_ROUTE_SERVER_CONNECTION_BACKLOG
    #define CSP_ROUTE_SERVER_CONNECTION_BACKLOG     10
#endif

/** @} */ // End of CSP Configuration Parameters

/* ======================================================================== */
/*                        INTERFACE CONFIGURATION                          */
/* ======================================================================== */

/**
 * @defgroup INTERFACE_CONFIG Interface Configuration
 * @brief Parameters for various CSP interfaces
 * @{
 */

#if CSP_ROUTE_SERVER_ENABLE_KISS_IFACE && ENABLE_HW_UART
/**
 * @brief KISS interface default name
 * @details Name used for KISS over UART interface
 */
#ifndef CSP_ROUTE_SERVER_KISS_IFACE_NAME
    #define CSP_ROUTE_SERVER_KISS_IFACE_NAME         CSP_IF_KISS_DEFAULT_NAME
#endif
#endif

#if CSP_ROUTE_SERVER_ENABLE_CAN_IFACE && ENABLE_HW_CAN
/**
 * @brief CAN interface default name
 * @details Name used for CAN interface
 */
#ifndef CSP_ROUTE_SERVER_CAN_IFACE_NAME
    #define CSP_ROUTE_SERVER_CAN_IFACE_NAME          CSP_IF_CAN_DEFAULT_NAME
#endif
#endif

#if CSP_ROUTE_SERVER_ENABLE_I2C_IFACE && ENABLE_HW_I2C
/**
 * @brief I2C interface default name
 * @details Name used for I2C interface
 */
#ifndef CSP_ROUTE_SERVER_I2C_IFACE_NAME
    #define CSP_ROUTE_SERVER_I2C_IFACE_NAME          CSP_IF_I2C_DEFAULT_NAME
#endif

/**
 * @brief I2C buffer size
 * @details Buffer size for I2C interface in bytes
 */
#ifndef CSP_ROUTE_SERVER_I2C_BUFFER_SIZE
    #define CSP_ROUTE_SERVER_I2C_BUFFER_SIZE         256
#endif
#endif

/** @} */ // End of Interface Configuration

/* ======================================================================== */
/*                        SHELL COMMAND CONFIGURATION                      */
/* ======================================================================== */

/**
 * @defgroup SHELL_CMD_CONFIG Shell Command Configuration
 * @brief Parameters for CSP shell commands
 * @{
 */

#if CSP_ROUTE_SERVER_ENABLE_SHELL_CMDS
/**
 * @brief Default CSP send timeout
 * @details Default timeout (in milliseconds) for csp_send command
 */
#ifndef CSP_ROUTE_SEND_CMD_DEFAULT_TIMEOUT
    #define CSP_ROUTE_SEND_CMD_DEFAULT_TIMEOUT      1000
#endif

/**
 * @brief Default CSP connection priority
 * @details Default priority for csp_send command connections
 */
#ifndef CSP_ROUTE_SEND_CMD_DEFAULT_PRIORITY
    #define CSP_ROUTE_SEND_CMD_DEFAULT_PRIORITY     CSP_PRIO_NORM
#endif
#endif

/** @} */ // End of Shell Command Configuration

/* ======================================================================== */
/*                        DEBUG CONFIGURATION                             */
/* ======================================================================== */

/**
 * @defgroup DEBUG_CONFIG Debug Configuration
 * @brief Debug and logging configuration
 * @{
 */

/**
 * @brief CSP debug level
 * @details Maximum debug level to output
 * @note Can be CSP_ERROR, CSP_WARN, CSP_INFO, CSP_DEBUG, or CSP_LOCK
 */
#ifndef CSP_ROUTE_SERVER_DEBUG_LEVEL
    #define CSP_ROUTE_SERVER_DEBUG_LEVEL             CSP_INFO
#endif

/** @} */ // End of Debug Configuration

#ifdef __cplusplus
}
#endif

#endif /* __CSP_ROUTE_SERVER_CONFIG_H */
