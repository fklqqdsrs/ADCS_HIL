#include "app_modules_config.h"
#if CSP_ENABLE_LOG

/*
 * console_be.c
 *
 *  Created on: Sep 18, 2025
 *      Author: Pongpot
 */

#include <hw_config.h>
#include <tsc_log_core.h>

#if TSC_LOG_BACKEND_USING_CONSOLE

#if defined(TSC_LOG_ASYNC_OUTPUT_BY_THREAD) && TSC_LOG_ASYNC_OUTPUT_THREAD_STACK < 384
#error "The thread stack size must more than 384 when using async output by thread (TSC_LOG_ASYNC_OUTPUT_BY_THREAD)"
#endif

static struct tsc_log_backend console = { 0 };

void tsc_log_console_backend_output(struct tsc_log_backend *backend, uint32_t level, const char *tag, bool is_raw,
        const char *log, size_t len)
{
	*(char*)(log+len) = 0;
	tsc_hw_console_output(log);
}

int tsc_log_console_backend_init(void)
{
    console.output = tsc_log_console_backend_output;

    tsc_log_backend_register(&console, "console", true);

    return 0;
}

#endif /* TSC_LOG_BACKEND_USING_CONSOLE */


#endif /* CSP_ENABLE_LOG */
