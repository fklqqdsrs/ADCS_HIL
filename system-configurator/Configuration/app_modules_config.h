/**
 ******************************************************************************
 * @file    app_modules_config.h
 * @brief   Master Configuration File for All Project Modules
 * @details This file centralizes all module enable/disable flags for the 
 *          entire project. Set values to 1 to enable, 0 to disable.
 *          This is the single source of truth for module configuration.
 ******************************************************************************
 * @attention
 *
 * This file should be modified to enable/disable modules at compile time.
 * Disabled modules will be completely excluded from the build, saving
 * flash memory and RAM.
 *
 ******************************************************************************
 */

#ifndef APP_MODULES_CONFIG_H
#define APP_MODULES_CONFIG_H

/* ======================================================================== */
/*                        HARDWARE MODULES                                  */
/* ======================================================================== */

/**
 * @brief Enable/Disable CAN hardware driver
 * @note When disabled, all CAN-related code will be excluded from build
 */

/**
 * @brief Enable/Disable UART hardware driver
 * @note When disabled, all UART-related code will be excluded from build
 */
#define ENABLE_HW_CAN            1
#define ENABLE_HW_I2C            1

#define ENABLE_HW_GPIO           1

#define ENABLE_HW_UART           1

/**
 * @brief Enable/Disable I2C hardware driver
 * @note When disabled, all I2C-related code will be excluded from build
 */
/**
 * @brief Enable/Disable GPIO hardware driver
 * @note Typically should remain enabled for most applications
 */
/* ======================================================================== */
/*                        SHELL/CLI MODULES                                */
/* ======================================================================== */

/**
 * @brief Enable/Disable Shell/Command Line Interface
 * @note Provides command-line interface for debugging and configuration
 */
#define ENABLE_SHELL             1



/* ======================================================================== */
/*                        MIDDLEWARE MODULES                               */
/* ======================================================================== */

/**
 * @brief Enable/Disable libcsp library
 * @note Required for most CSP-related functionality
 */
#define ENABLE_S25FL128L         0

#define ENABLE_LITTLEFS          0

#define CSP_ENABLE_FILE          0

#define ENABLE_LIBCSP            1

#define CSP_ENABLE_CONFIG        0

/**
 * @brief Enable/Disable LittleFS file system
 * @note Required for file operations and persistent storage
 */
/**
 * @brief Enable/Disable S25FL128L Flash driver
 * @note Required if using external SPI flash
 */
/* ======================================================================== */
/*                        CSP APPLICATION MODULES                         */
/* ======================================================================== */

/**
 * @brief Enable/Disable CSP File Service
 * @note Provides file transfer capabilities over CSP
 */
/**
 * @brief Enable/Disable CSP Configuration Service
 * @note Provides remote configuration capabilities
 */
/**
 * @brief Enable/Disable CSP Thread Service
 * @note Provides thread/task management over CSP
 */
#define CSP_ENABLE_THREAD        1

/**
 * @brief Enable/Disable CSP Logging Service
 * @note Provides distributed logging capabilities
 */
#define CSP_ENABLE_LOG           0

/**
 * @brief Enable/Disable CSP Data Monitoring Service
 * @note Provides real-time data monitoring
 */
#define CSP_ENABLE_DATA_MON      1

#define CSP_ENABLE_ROUTE         1

/**
 * @brief Enable/Disable CSP Routing Service
 * @note Provides packet routing capabilities
 */
/**
 * @brief Enable/Disable CSP Register Service
 * @note Provides register access over CSP
 */
#define CSP_ENABLE_REGISTER      1

/* ======================================================================== */
/*                        TIME MANAGEMENT MODULES                          */
/* ======================================================================== */
#define ENABLE_TIME_RTC         1
/**
 * @brief Enable/Disable GNSS time synchronization
 * @note Provides GPS-based time synchronization
 */
#define ENABLE_TIME_GNSS         0

/**
 * @brief Enable/Disable CTP (CSP Time Protocol)
 * @note Provides time synchronization over CSP
 */
#define ENABLE_TIME_CTP          0

/**
 * @brief Enable/Disable PPS (Pulse Per Second) interrupt
 * @note Requires external PPS source for precision timing
 */
#define ENABLE_TIME_PPS          0

/* ======================================================================== */
/*                        AUTOMATION MODULES                              */
/* ======================================================================== */

/**
 * @brief Enable/Disable Script Manager
 * @note Provides script execution and scheduling
 */
#define ENABLE_SCRIPT_MANAGER    0


/* ======================================================================== */
/*                        BOOTLOADER VALIDATION                             */
/* ======================================================================== */

/**
 * @brief Enable/Disable Bootloader Validator
 * @note Provides bootloader validation capabilities
 */
#define ENABLE_BOOTLOADER_VALIDATOR    0

/* ======================================================================== */
/*                        VALIDATION CONFIGURATION                         */
/* ======================================================================== */

/* Validate critical configurations */
#if !ENABLE_LIBCSP && (CSP_ENABLE_FILE || CSP_ENABLE_CONFIG || CSP_ENABLE_THREAD || \
                       CSP_ENABLE_LOG || CSP_ENABLE_DATA_MON || CSP_ENABLE_ROUTE || \
                       CSP_ENABLE_REGISTER || ENABLE_TIME_CTP)
    #error "CSP applications require ENABLE_LIBCSP to be set to 1"
#endif

#if ENABLE_TIME_GNSS && !ENABLE_HW_UART
    #warning "Time GNSS requires ENABLE_HW_UART for GNSS communication"
#endif

#if !ENABLE_LITTLEFS && (CSP_ENABLE_FILE )
    #error "File operations require ENABLE_LITTLEFS to be set to 1"
#endif

#endif /* APP_MODULES_CONFIG_H */
