#include "app_modules_config.h"
#if CSP_ENABLE_THREAD

//! @file thread_manager_cli.c
//! @brief C Source File Template

// --------------------------------------------------------------------------------------------------- //
// COMPANY NAME : IOT POW.
//
// INTRODUCTION : CSP thread manager command line.
//
// AUTHOR(S) : Pongpot Chaiboonuang
// CONTACT E-MAIL ADDRESS : pongpot.c@gmail.com
//
// REFERENCE DOCUMENT :
// N/A
//
// NOTE :
// N/A
//
// --------------------------------------------------------------------------------------------------- //

// ---------- I M P O R T A N T   N O T E ------------------------------------------------------------ //
//
// --------------------------------------------------------------------------------------------------- //
// N/A

// ---------- (1) LOCAL PREDEFINE -------------------------------------------------------------------- //
#define __THIS_FILE_NAME__		"thread_manager_cli.c"

// ---------- (2) STANDARD LIBRARY INCLUDE ----------------------------------------------------------- //
#include <kservice.h>
#ifdef TSC_USING_FINSH
// ---------- (3) USER INCLUDE ----------------------------------------------------------------------- //
#include <thread_manager_core.h>

// ---------- (4) EXTERN OBJECT ---------------------------------------------------------------------- //
extern uint32_t thread_count;
extern managed_thread_t thread_table[];

// Forward declaration to access internal manager function
extern managed_thread_t *find_thread(const char *name);

// ---------- (5) INTERFACE FUNCTION ----------------------------------------------------------------- //
// N/A

// ---------- (6) DATA ------------------------------------------------------------------------------- //
// N/A

// ---------- (7) PRIVATE PROGRAMMING DEFINE --------------------------------------------------------- //
// N/A

// ---------- (8) PRIVATE TYPEDEF -------------------------------------------------------------------- //
// N/A

// ---------- (9) PRIVATE FUNCTION ------------------------------------------------------------------- //
// N/A

// ---------- (10) PRIVATE DATA ----------------------------------------------------------------------- //
// N/A

// ---------- (11) PRIVATE MACRO DEFINITION ---------------------------------------------------------- //
// N/A

// ---------- (12) SOURCE FILE IMPLEMENTATION -------------------------------------------------------- //

/**
 * @brief Check if a thread with the given name exists.
 *
 * @param name Thread name to search for
 * @return 1 if found, 0 otherwise
 */
int is_thread_exist(const char *name)
{
    return find_thread(name) != NULL;
}

/**
 * @brief Configure watchdog settings for a thread.
 *
 * Usage: thread_config <name> <timeout_ms> <max_retries> <watchdog_enable>
 */
static void thread_config_cmd(int argc, char **argv)
{
    if (argc != 5)
    {
    	tsc_kprintf("Usage: thread_config <name> <timeout_ms> <max_retries> <wd_enable>\n");
        return;
    }

    const char *name = argv[1];
    uint32_t timeout = atoi(argv[2]);
    uint8_t retry = atoi(argv[3]);
    uint8_t enable = atoi(argv[4]) ? 1 : 0;

    managed_thread_t *t = find_thread(name);
    if (!t)
    {
    	tsc_kprintf("Error: thread '%s' not found.\n", name);
        return;
    }

    t->watchdog_timeout_ms = timeout;
    t->max_retry_limit = retry;
    t->watchdog_enabled = enable;

    tsc_kprintf("Thread '%s' config updated:\n", t->name);
    tsc_kprintf("  Watchdog Timeout: %lu ms\n", (unsigned long)t->watchdog_timeout_ms);
    tsc_kprintf("  Retry Limit:      %u\n", t->max_retry_limit);
    tsc_kprintf("  Watchdog Enable:  %s\n", t->watchdog_enabled ? "ON" : "OFF");
}

/**
 * @brief Start a registered thread.
 *
 * @param argc Number of arguments
 * @param argv Argument list: name
 */
static void thread_start_cmd(int argc, char **argv)
{
    if (argc != 2)
    {
        tsc_kprintf("Usage: thread_start <name>\nTry 'thread_list' to see names.\n");
        return;
    }

    if (!is_thread_exist(argv[1]))
    {
        tsc_kprintf("Error: thread '%s' not found.\n", argv[1]);
        return;
    }

    int ret = thread_start(argv[1]);
    tsc_kprintf("Thread start: %s\n", ret == 0 ? "OK" : "Fail");
}

/**
 * @brief Stop (terminate) a running thread.
 *
 * @param argc Number of arguments
 * @param argv Argument list: name
 */
static void thread_stop_cmd(int argc, char **argv)
{
    if (argc != 2)
    {
        tsc_kprintf("Usage: thread_stop <name>\n");
        return;
    }

    if (!is_thread_exist(argv[1]))
    {
        tsc_kprintf("Error: thread '%s' not found.\n", argv[1]);
        return;
    }

    int ret = thread_stop(argv[1]);
    tsc_kprintf("Thread stop: %s\n", ret == 0 ? "OK" : "Fail");
}

/**
 * @brief Manually restart a thread.
 *
 * @param argc Number of arguments
 * @param argv Argument list: name
 */
static void thread_restart_cmd(int argc, char **argv)
{
    if (argc != 2)
    {
        tsc_kprintf("Usage: thread_restart <name>\n");
        return;
    }

    if (!is_thread_exist(argv[1]))
    {
        tsc_kprintf("Error: thread '%s' not found.\n", argv[1]);
        return;
    }

    int ret = thread_restart(argv[1]);
    tsc_kprintf("Thread restart: %s\n", ret == 0 ? "OK" : "Fail");
}

/**
 * @brief Show status of a specific thread.
 *
 * @param argc Number of arguments
 * @param argv Argument list: name
 */
static void thread_stat_cmd(int argc, char **argv)
{
    if (argc != 2)
    {
        tsc_kprintf("Usage: thread_stat <name>\n");
        return;
    }

    if (!is_thread_exist(argv[1]))
    {
        tsc_kprintf("Error: thread '%s' not found.\n", argv[1]);
        return;
    }

    thread_print_status(argv[1]);
}

/**
 * @brief Show status of all registered threads.
 *
 * @param argc Number of arguments (unused)
 * @param argv Argument list (unused)
 */
static void thread_stat_all(int argc, char **argv)
{
    thread_print_all_status();
}

/**
 * @brief List names of all registered threads.
 *
 * @param argc Number of arguments (unused)
 * @param argv Argument list (unused)
 */
static void thread_list(int argc, char **argv)
{
    tsc_kprintf("Registered threads:\n");
    for (uint32_t i = 0; i < thread_count; i++)
    {
        tsc_kprintf("  %s\n", thread_table[i].name);
    }
}

/**
 * @brief Show help for all thread commands.
 *
 * @param argc Number of arguments (unused)
 * @param argv Argument list (unused)
 */
static void thread_help(int argc, char **argv)
{
    tsc_kprintf("Thread Manager CLI Help:\n");
    tsc_kprintf("  thread_start <name>                 - Start a thread\n");
    tsc_kprintf("  thread_stop <name>                  - Stop a thread\n");
    tsc_kprintf("  thread_restart <name>               - Restart a thread\n");
    tsc_kprintf("  thread_stat <name>                  - Show status of one thread\n");
    tsc_kprintf("  thread_stat_all                     - Show status of all threads\n");
    tsc_kprintf("  thread_list                         - List registered thread names\n");
}

/* -------------------------------------------------------------------------- */
/* Command Registration (CLI binding)                                         */
/* -------------------------------------------------------------------------- */
TSC_CMD_EXPORT_ALIAS(thread_config_cmd, thread_config, Set timeout retry and enable watchdog for a thread);
TSC_CMD_EXPORT_ALIAS(thread_start_cmd, thread_start, Start a thread);
TSC_CMD_EXPORT_ALIAS(thread_stop_cmd, thread_stop, Stop a thread);
TSC_CMD_EXPORT_ALIAS(thread_restart_cmd, thread_restart, Restart a thread);
TSC_CMD_EXPORT_ALIAS(thread_stat_cmd, thread_stat, Show one thread status);
TSC_CMD_EXPORT(thread_stat_all, Show all thread status);
TSC_CMD_EXPORT(thread_list, List all registered thread names);
TSC_CMD_EXPORT(thread_help, Show all thread manager commands);

#endif // TSC_USING_FINSH

// ---------- (13) END OF SOURCE FILE IMPLEMENTATION ------------------------------------------------- //
// SOURCE FORMAT REVISION B


#endif /* CSP_ENABLE_THREAD */
