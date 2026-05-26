#include "app_modules_config.h"
#if CSP_ENABLE_THREAD

//! @file thread_manager.c
//! @brief C Source File Template

// --------------------------------------------------------------------------------------------------- //
// COMPANY NAME : IOT POW.
//
// INTRODUCTION : Thread manager application.
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
#define __THIS_FILE_NAME__		"thread_manager.c"

// ---------- (2) STANDARD LIBRARY INCLUDE ----------------------------------------------------------- //
#include <kservice.h>

// ---------- (3) USER INCLUDE ----------------------------------------------------------------------- //
#include <thread_manager_core.h>

#define LOG_TAG		"thread_mgr"
#define LOG_LVL		DBG_LOG
#include <tsc_log_core.h>

// ---------- (4) EXTERN OBJECT ---------------------------------------------------------------------- //
extern const thread_static_entry_t __start_thread_table[];
extern const thread_static_entry_t __stop_thread_table[];

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
static osTimerId_t thread_watchdog_timer_id = NULL;
managed_thread_t thread_table[CSP_THREAD_MANAGER_MAX_NUM];
uint32_t thread_count = 0;

// ---------- (11) PRIVATE MACRO DEFINITION ---------------------------------------------------------- //
// N/A

// ---------- (12) SOURCE FILE IMPLEMENTATION -------------------------------------------------------- //
/**
 * @brief Start and register a thread from a static descriptor
 */
int thread_start_from_static(const thread_static_entry_t *desc)
{
    osThreadAttr_t attr = {
        .name = desc->name,
        .stack_size = desc->stack_size,
        .priority = desc->priority
    };

    int ret = thread_register(desc->name, desc->entry, (void *)desc->arg,
                              &attr, desc->watchdog_timeout_ms,
                              desc->max_retry_limit, desc->watchdog_enabled,
							  desc->autorun);

    if (ret == 0 && desc->autorun)
    {
    	LOG_I("thread_start_from_static: Auto-starting thread [%s]", desc->name);
        return thread_start(desc->name);
    }
    return ret;
}

/**
 * @brief Register and start all static threads marked for autorun
 */
void thread_autoreg_from_flash(void)
{
    LOG_I("thread_autoreg_from_flash: Loading static thread table...");
    const thread_static_entry_t *entry = __start_thread_table;

    while (entry < __stop_thread_table)
    {
        int ret = thread_start_from_static(entry);
        if (ret == 0)
        {
            LOG_D("thread_autoreg_from_flash: Thread [%s] registration successful", entry->name);
        }
        else
        {
            LOG_E("thread_autoreg_from_flash: Thread [%s] registration failed (err=%d)", entry->name, ret);
        }
        entry++;
    }
}

/**
 * @brief Find a managed thread by name.
 */
managed_thread_t *find_thread(const char *name) {
    for (uint32_t i = 0; i < thread_count; i++) {
        if (strcmp(thread_table[i].name, name) == 0) return &thread_table[i];
    }
    return NULL;
}

/**
 * @brief Callback function for the watchdog timer.
 */
static void thread_watchdog_timer_cb(void *arg) {
    (void)arg;
    thread_manager_tick();
}

/**
 * @brief Initialize the thread manager and watchdog timer.
 */
void thread_manager_init(void) {
    LOG_I("thread_manager_init: Initializing Thread Manager...");
    memset(thread_table, 0, sizeof(thread_table));
    thread_count = 0;

    thread_watchdog_timer_id = osTimerNew(thread_watchdog_timer_cb,
                                           osTimerPeriodic,
                                           NULL,
                                           NULL);
    if (thread_watchdog_timer_id) {
        LOG_D("thread_manager_init: Starting watchdog timer (100ms interval)");
        osTimerStart(thread_watchdog_timer_id, 100); 
    } else {
        LOG_E("thread_manager_init: Failed to create watchdog timer!");
    }

    thread_autoreg_from_flash();
}

/**
 * @brief Register a new thread with watchdog settings.
 */
int thread_register(const char *name, void (*entry)(void *), void *arg, osThreadAttr_t *attr,
                    uint32_t timeout_ms, uint32_t max_retries, uint8_t enable_watchdog, uint8_t autorun)
{
    if (thread_count >= CSP_THREAD_MANAGER_MAX_NUM) {
        LOG_E("thread_register: Manager full! Cannot register [%s]", name);
        return -1;
    }

    managed_thread_t *t = &thread_table[thread_count++];
    t->name = name;
    t->entry = entry;
    t->arg = arg;
    t->attr = *attr;
    t->enabled = 0;
    t->last_heartbeat = 0;
    t->retry_count = 0;
    t->watchdog_timeout_ms = timeout_ms;
    t->max_retry_limit = max_retries;
    t->watchdog_enabled = enable_watchdog ? 1 : 0;
    t->autorun = autorun;

    LOG_D("thread_register: [%s] registered (WD: %s, %lu ms)", name, 
               enable_watchdog ? "YES" : "NO", (unsigned long)timeout_ms);
    return 0;
}

/**
 * @brief Unregister a thread and remove it from the manager.
 */
int thread_unregister(const char *name) {
    managed_thread_t *t = find_thread(name);
    if (!t) return -1;

    LOG_I("thread_unregister: Removing thread [%s]", name);
    if (t->enabled) {
        LOG_W("thread_unregister: Thread [%s] is active, terminating first", name);
        osThreadTerminate(t->thread_id);
    }

    // Shift remaining threads down to keep array compact
    for (uint32_t i = 0; i < thread_count; i++) {
        if (&thread_table[i] == t) {
            for (uint32_t j = i; j < thread_count - 1; j++) {
                thread_table[j] = thread_table[j + 1];
            }
            break;
        }
    }

    thread_count--;
    return 0;
}

/**
 * @brief Start a registered thread by name.
 */
int thread_start(const char *name) {
    managed_thread_t *t = find_thread(name);
    if (!t) {
        LOG_E("thread_start: Thread [%s] not found", name);
        return -1;
    }
    if (t->enabled) {
        LOG_W("thread_start: Thread [%s] is already running", name);
        return -1;
    }

    LOG_I("thread_start: Spawning thread [%s] (stack: %u, priority: %u)", 
               name, (unsigned int)t->attr.stack_size, (unsigned int)t->attr.priority);
    t->thread_id = osThreadNew(t->entry, t->arg, &t->attr);
    if (t->thread_id == NULL) {
        LOG_E("thread_start: Failed to create OS thread [%s]", name);
        return -2;
    }

    t->last_heartbeat = osKernelGetTickCount();
    t->enabled = 1;
    t->retry_count = 0;

    return 0;
}

/**
 * @brief Stop (terminate) a thread by name.
 */
int thread_stop(const char *name) {
    managed_thread_t *t = find_thread(name);
    if (!t || !t->enabled) {
        LOG_W("thread_stop: Thread [%s] not found or not running", name);
        return -1;
    }

    LOG_I("thread_stop: Terminating thread [%s]", name);
    osThreadTerminate(t->thread_id);
    t->enabled = 0;
    return 0;
}


/**
 * @brief Restart a thread manually by name.
 */
int thread_restart(const char *name) {
    managed_thread_t *t = find_thread(name);
    if (!t) return -1;

    LOG_I("thread_restart: Restarting thread [%s]", name);
    if (t->enabled) {
        osThreadTerminate(t->thread_id);
        t->enabled = 0;
    }

    t->thread_id = osThreadNew(t->entry, t->arg, &t->attr);
    if (!t->thread_id) {
        LOG_E("thread_restart: Failed to restart thread [%s]", name);
        return -2;
    }

    t->last_heartbeat = osKernelGetTickCount();
    t->enabled = 1;
    t->retry_count = 0;

    return 0;
}

/**
 * @brief Kick the watchdog for a running thread to update its heartbeat.
 */
void thread_kick_watchdog(const char *name) {
    managed_thread_t *t = find_thread(name);
    if (t && t->enabled) {
        t->last_heartbeat = osKernelGetTickCount();
    }
}

/**
 * @brief Periodic function called by timer to monitor all threads.
 */
void thread_manager_tick(void)
{
    uint32_t now = osKernelGetTickCount();

    for (uint32_t i = 0; i < thread_count; i++) {
        managed_thread_t *t = &thread_table[i];
        if (!t->enabled || !t->watchdog_enabled)
            continue;

        if ((now - t->last_heartbeat) > t->watchdog_timeout_ms) {
            LOG_E("thread_manager_tick: Watchdog TIMEOUT for thread [%s] (last kick: %lu, now: %lu)", 
                       t->name, (unsigned long)t->last_heartbeat, (unsigned long)now);
            
            osThreadTerminate(t->thread_id);
            t->enabled = 0;

            if ((t->retry_count < t->max_retry_limit) || (CSP_THREAD_MAX_RETRY == t->max_retry_limit)) {
                LOG_W("thread_manager_tick: Attempting automatic restart for [%s] (retry %u/%u)", 
                           t->name, t->retry_count + 1, t->max_retry_limit);
                t->thread_id = osThreadNew(t->entry, t->arg, &t->attr);
                if (t->thread_id) {
                    t->last_heartbeat = osKernelGetTickCount();
                    t->enabled = 1;
                    t->retry_count++;
                    LOG_I("thread_manager_tick: Thread [%s] restarted successfully", t->name);
                } else {
                    LOG_E("thread_manager_tick: Failed to restart thread [%s]!", t->name);
                }
            } else {
                LOG_E("thread_manager_tick: Thread [%s] exceeded retry limit. Giving up.", t->name);
            }
        }
    }
}

/**
 * @brief Get the managed_thread_t structure of a single thread.
 */
managed_thread_t* thread_get_manage_thread(const char *name)
{
    managed_thread_t *t = find_thread(name);
    if (!t)
    {
    	LOG_W("thread_get_manage_thread: Thread [%s] not found", name);
        return NULL;
    }
    return t;
}

/**
 * @brief Print the status of a single thread in human-readable format.
 *
 * @param name Name of the thread
 */
void thread_print_status(const char *name)
{
    managed_thread_t *t = find_thread(name);
    if (!t)
    {
    	tsc_kprintf("Thread '%s' not found.\n", name);
        return;
    }

    tsc_kprintf("[Thread: %s] Status:\n", t->name);
    tsc_kprintf("  Enabled:       %s\n", t->enabled ? "Yes" : "No");
    tsc_kprintf("  Retry Count:   %lu / %lu\n", (unsigned long)t->retry_count, (unsigned long)t->max_retry_limit);
    tsc_kprintf("  Last Kick:     %lu ms\n", (unsigned long)t->last_heartbeat);
    tsc_kprintf("  Stack Size:    %u\n", (unsigned int)t->attr.stack_size);
    tsc_kprintf("  Priority:      %u\n", (unsigned int)t->attr.priority);
    tsc_kprintf("  Watchdog:      %s\n", t->watchdog_enabled ? "Enabled" : "Disabled");
    if (t->watchdog_enabled)
    {
    	tsc_kprintf("    Timeout:     %lu ms\n", (unsigned long)t->watchdog_timeout_ms);
    }
}

/**
 * @brief Print the status of all registered threads in a table format.
 */
void thread_print_all_status(void) {
	tsc_kprintf("| %-16s | %-6s | %-21s | %-14s | %-10s | %-8s | %-9s | %-7s | %-7s |\r\n", "Name", "State", "Retry", "Last Kick (ms)", "Stack", "Priority", "Watchdog", "Timeout", "Autorun");
	tsc_kprintf("|------------------|--------|-----------------------|----------------|------------|----------|-----------|---------|---------|\r\n");

    for (uint32_t i = 0; i < thread_count; i++) {
        managed_thread_t *t = &thread_table[i];
        tsc_kprintf("| %-16s | %-6s | %10lu/%-10lu | %-14lu | %-10u | %-8u | %-9s | %-7lu | %-7s |\r\n",
                	t->name,
					t->enabled ? "ON" : "OFF",
					(unsigned long)t->retry_count,
					(unsigned long)t->max_retry_limit,
					(unsigned long)t->last_heartbeat,
					(unsigned int)t->attr.stack_size,
					(unsigned int)t->attr.priority,
					t->watchdog_enabled ? "ENABLED" : "DISABLED",
					t->watchdog_enabled ? (unsigned long)t->watchdog_timeout_ms : 0,
					t->autorun ? "Yes" : "NO");
    }

    tsc_kprintf("=== End ===\n\n");
}

// ---------- (13) END OF SOURCE FILE IMPLEMENTATION ------------------------------------------------- //
// SOURCE FORMAT REVISION B


#endif /* CSP_ENABLE_THREAD */
