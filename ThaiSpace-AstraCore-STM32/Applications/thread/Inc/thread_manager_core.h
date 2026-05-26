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

#ifndef _THREAD_MANAGER_H
#define _THREAD_MANAGER_H

#if defined __cplusplus
extern "C" {
#endif
// ---------- (1) SYSTEM INCLUDE --------------------------------------------------------------------- //
#include "cmsis_os2.h"
#include <csp/csp.h>

// ---------- (2) EXTERNAL MODULE INCLUDE ------------------------------------------------------------ //
// N/A

// ---------- (3) PUBLIC PROGRAMMING DEFINE ---------------------------------------------------------- //
#define CSP_THREAD_MAX_RETRY			0xffffffffUL

// ---------- (4) ENUMERATOR DEFINITION -------------------------------------------------------------- //
// N/A

// ---------- (5) STRUCT OR UNION DATA TYPE DEFINITION ----------------------------------------------- //
// N/A

// ---------- (6) TYPEDEF DATA TYPE DEFINITION ------------------------------------------------------- //
/**
 * @brief Thread entry definition used by the thread manager
 */
typedef struct {
    const char *name;
    osThreadId_t thread_id;
    osThreadAttr_t attr;
    void (*entry)(void *arg);
    void *arg;

    volatile uint32_t last_heartbeat;
    uint32_t retry_count;
    uint8_t enabled;

    uint32_t watchdog_timeout_ms;
    uint32_t max_retry_limit;
    uint8_t watchdog_enabled;       	/**< 1 = enabled, 0 = disabled */
    uint8_t autorun;
} managed_thread_t;

/**
 * @brief Thread registration entry placed in flash (static)
 */
typedef struct {
    const char *name;                   /**< Thread name */
    void (*entry)(void *arg);          	/**< Thread function */
    void *arg;							/**< Thread input argument */
    uint32_t stack_size;                /**< Stack size in bytes */
    uint32_t priority;                  /**< CMSIS thread priority */
    uint32_t watchdog_timeout_ms;       /**< Watchdog timeout */
    uint32_t max_retry_limit;           /**< Max restart attempts */
    uint8_t watchdog_enabled;           /**< Watchdog enable flag */
    uint8_t autorun;                    /**< Run at startup if set */
} thread_static_entry_t;

/**
 * @brief Thread status structure for get current status of thread
 */
typedef struct {
    char name[CSP_THREAD_MANAGER_NAME_LENGTH];
    uint32_t stack_size;
    uint32_t priority;

    uint32_t last_heartbeat;
    uint32_t retry_count;
    uint8_t enabled;

    uint32_t watchdog_timeout_ms;
    uint32_t max_retry_limit;
    uint8_t watchdog_enabled;
    uint8_t autorun;
} thread_status_runtime_t;

/**
 * @brief Thread config structure for runtime configuration
 */
typedef struct {
    char name[CSP_THREAD_MANAGER_NAME_LENGTH];
    uint32_t stack_size;
    uint32_t priority;
    uint32_t watchdog_timeout_ms;
    uint32_t max_retry_limit;
    uint8_t watchdog_enabled;
} thread_config_runtime_t;

// ---------- (7) PUBLIC MACRO DEFINITION ------------------------------------------------------------ //

#define TSC_THREAD_SECTION __attribute__((used, section(".thread_table")))

/**
 * @brief Macro to declare a static thread registration entry in flash
 */
#define TSC_THREAD_REGISTER(t_name, t_entry, t_num, t_arg, stack, prio, timeout, retry, wd, run)  \
    static const thread_static_entry_t __thread_##t_entry##_##t_num TSC_THREAD_SECTION = { \
        .name = t_name,                                                    \
        .entry = t_entry,                                                  \
		.arg = t_arg,													   \
        .stack_size = stack,                                               \
        .priority = prio,                                                  \
        .watchdog_timeout_ms = timeout,                                    \
        .max_retry_limit = retry,                                          \
        .watchdog_enabled = wd,                                            \
        .autorun = run                                                     \
    };


// ---------- (8) EXTERN FUNCTION -------------------------------------------------------------------- //
int is_thread_exist(const char *name);

// ---------- (9) EXTERN VARIABLE -------------------------------------------------------------------- //
// N/A

// ---------- (10) CLASS DECLARATION ----------------------------------------------------------------- //
void thread_manager_init(void);
int thread_register(const char *name, void (*entry)(void *), void *arg, osThreadAttr_t *attr,
                    uint32_t timeout_ms, uint32_t max_retries, uint8_t enable_watchdog, uint8_t autorun);
int thread_unregister(const char *name);
int thread_start(const char *name);
int thread_stop(const char *name);
int thread_restart(const char *name);
void thread_kick_watchdog(const char *name);
static inline void thread_wd_kick_self(void) {
    osThreadId_t tid = osThreadGetId();
    const char *name = osThreadGetName(tid);
    if (name) {
        thread_kick_watchdog(name);
    } 
}

void thread_manager_tick(void); // Call every N ms from timer

managed_thread_t* thread_get_manage_thread(const char *name);

void thread_print_status(const char *name);
void thread_print_all_status(void);
// ---------- (11) END OF CLASS DECLARATION ---------------------------------------------------------- //
#if defined __cplusplus
}
#endif

#endif  // #ifndef _THREAD_MANAGER_H
