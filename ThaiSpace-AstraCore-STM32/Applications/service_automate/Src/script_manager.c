#include <app_modules_config.h>

#if ENABLE_SCRIPT_MANAGER

/**
 * @file script_manager.c
 * @author chatu
 * @brief Manages the execution of script commands, both local and remote.
 */
#include "script_manager.h"
#include "script_service_init.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <csp/csp.h>
#include <csp/arch/csp_thread.h>
#include "fatfs.h"
#include "finsh.h"

/* ===== Configuration Macros ===== */
#ifndef SCRIPT_LOOP_TASK_STACK_SIZE
#define SCRIPT_LOOP_TASK_STACK_SIZE     4096
#endif

#ifndef SCRIPT_CMD_GAP_MS
#define SCRIPT_CMD_GAP_MS               100
#endif

#ifndef SCRIPT_REMOTE_TIMEOUT_MS
#define SCRIPT_REMOTE_TIMEOUT_MS        1000
#endif

#ifndef SCRIPT_MAX_FILENAME_LEN
#define SCRIPT_MAX_FILENAME_LEN         128
#endif

#ifndef SCRIPT_MAX_RUNTIME_MS
#define SCRIPT_MAX_RUNTIME_MS           0  /* 0 = disabled, set to e.g., 30000 for 30s timeout */
#endif

#define LOG_TAG		"script_mgr"
#define LOG_LVL		DBG_LOG
#include <tsc_log_core.h>

/* ===== Logging Macros ===== */
#define SCRIPT_LOG_ERROR(...)   LOG_E(__VA_ARGS__)
#define SCRIPT_LOG_WARN(...)    LOG_W(__VA_ARGS__)
#define SCRIPT_LOG_INFO(...)    LOG_I(__VA_ARGS__)
#define SCRIPT_LOG_DEBUG(...)   LOG_D(__VA_ARGS__)

/* Platform-specific time function */
#include "cmsis_os2.h"
#define script_platform_millis()    osKernelGetTickCount()

/* ===== Thread-safe loop state using atomic operations ===== */
#include <stdatomic.h>
static volatile atomic_bool g_loop_running = ATOMIC_VAR_INIT(false);

// --- Command Dispatcher for Remote Commands ---
typedef void (*remote_cmd_handler_t)(const char *args);

typedef struct {
    const char *command_name;
    remote_cmd_handler_t handler;
} remote_command_entry_t;

// Forward declarations for handlers
static void handle_run_remote(const char *args) { script_run_local(args); }
static void handle_loop_remote(const char *args) { script_loop_local(args); }
static void handle_stop_remote(const char *args) { script_stop_local(args); }

static const remote_command_entry_t remote_command_table[] = {
    {"R", handle_run_remote},
    {"L", handle_loop_remote},
    {"S", handle_stop_remote},
    {NULL, NULL} // End of table marker
};

typedef int (*cmd_func_t)(int argc, char **argv);

static int build_argv(char *buf, const char *cmd, const char *args, char **argv, int maxv) {
    int argc = 0;
    argv[argc++] = (char*)cmd;
    if (args && *args) {
        strncpy(buf, args, 255);
        buf[255] = '\0';
        char *p = strtok(buf, " ");
        while (p && argc < maxv) {
            argv[argc++] = p;
            p = strtok(NULL, " ");
        }
    }
    return argc;
}

/* Find a finsh-exported command by name and return its function pointer. */
static cmd_func_t finsh_resolve_cmd(const char *name) {
    struct finsh_syscall *index;
    for (index = _syscall_table_begin; index < _syscall_table_end; ) {
        if (index->name && (strcmp(index->name, name) == 0)) {
            /* finsh stores as syscall_func (long (*)(void)), but real handlers use int(argc,argv) */
            return (cmd_func_t)(index->func);
        }
        FINSH_NEXT_SYSCALL(index);
    }
    return NULL;
}

void execute_script_line(const char *command, const char *args) {
    cmd_func_t func = finsh_resolve_cmd(command);
    if (func) {
        char buf[256];
        char *argv[16];
        int argc = build_argv(buf, command, args, argv, 16);

        SCRIPT_LOG_DEBUG("%s%s%s\r\n", command, args ? " " : "", args ? args : "");
        int rc = func(argc, argv);
        (void)rc;
    } else {
        SCRIPT_LOG_ERROR("Unknown command '%s'\r\n", command);
    }
}

void script_run_local(const char *filename) {
    if (!filename || strlen(filename) == 0) {
        SCRIPT_LOG_ERROR("Usage: run <filename.txt>\r\n");
        return;
    }
    
    /* Path traversal validation */
    if (strstr(filename, "..") != NULL || strstr(filename, "/") != NULL || strstr(filename, "\\") != NULL) {
        SCRIPT_LOG_ERROR("Invalid path: %s (path traversal not allowed)\r\n", filename);
        return;
    }
    
    /* Validate filename length */
    if (strlen(filename) >= SCRIPT_MAX_FILENAME_LEN) {
        SCRIPT_LOG_ERROR("Filename too long (max %d chars)\r\n", SCRIPT_MAX_FILENAME_LEN - 1);
        return;
    }
    
    FIL file;
    if (f_open(&file, filename, FA_READ) != FR_OK) {
        SCRIPT_LOG_ERROR("Could not open script file '%s'\r\n", filename);
        return;
    }
    
    /* Track execution time for timeout */
    uint32_t start_time = script_platform_millis();
    uint32_t timeout_ms = SCRIPT_MAX_RUNTIME_MS;
    bool timeout_enabled = (timeout_ms > 0);
    
    char line_buffer[256];
    int line_count = 0;
    SCRIPT_LOG_INFO("--- Running script: %s ---\r\n", filename);
    
    while (f_gets(line_buffer, sizeof(line_buffer), &file)) {
        /* Check timeout */
        if (timeout_enabled) {
            uint32_t elapsed = script_platform_millis() - start_time;
            if (elapsed >= timeout_ms) {
                SCRIPT_LOG_ERROR("Script timeout after %lu ms\r\n", (unsigned long)timeout_ms);
                f_close(&file);
                return;
            }
        }
        
        line_count++;
        line_buffer[strcspn(line_buffer, "\r\n")] = '\0';
        char *command = strtok(line_buffer, " ");
        char *args = strtok(NULL, "");
        if (command && *command) {
            execute_script_line(command, args);
            csp_sleep_ms(SCRIPT_CMD_GAP_MS);
        }
    }
    SCRIPT_LOG_INFO("--- Script finished: %s (%d lines) ---\r\n", filename, line_count);
    f_close(&file);
}

CSP_DEFINE_TASK(task_script_looper) {
    char* args_copy = (char*)param;
    char* filename = strtok(args_copy, " ");
    char* delay_str = strtok(NULL, " ");
    int delay_ms = delay_str ? atoi(delay_str) : 1000;

    if (!filename) {
        free(args_copy);
        atomic_store(&g_loop_running, false);
        csp_thread_exit();
        return CSP_TASK_RETURN;
    }

    /* Use static buffer instead of strdup to avoid memory leak */
    char filename_copy[SCRIPT_MAX_FILENAME_LEN];
    strncpy(filename_copy, filename, sizeof(filename_copy) - 1);
    filename_copy[sizeof(filename_copy) - 1] = '\0';

    while(atomic_load(&g_loop_running)) {
        SCRIPT_LOG_INFO("\n--- Running script loop: %s ---\r\n", filename_copy);
        script_run_local(filename_copy);
        csp_sleep_ms(delay_ms > 0 ? delay_ms : 1000);
    }

    /* Only free args_copy, no need to free filename_copy (static buffer) */
    free(args_copy);
    atomic_store(&g_loop_running, false);
    csp_thread_exit();
    return CSP_TASK_RETURN;
}

// --- Public Functions ---

int script_send_remote_command(int node, const char *command) {
    csp_conn_t *conn = csp_connect(CSP_PRIO_NORM, node, SCRIPT_SERVICE_PORT, 
                                   SCRIPT_REMOTE_TIMEOUT_MS, CSP_SO_NONE);
    if (conn == NULL) {
        SCRIPT_LOG_ERROR("Connection to node %d failed\r\n", node);
        return -1;
    }

    csp_packet_t *packet = csp_buffer_get(strlen(command) + 1);
    if (packet == NULL) {
        SCRIPT_LOG_ERROR("Failed to get CSP buffer\r\n");
        csp_close(conn);
        return -1;
    }

    strcpy((char *)packet->data, command);
    packet->length = strlen(command) + 1;

    if (!csp_send(conn, packet, SCRIPT_REMOTE_TIMEOUT_MS)) {
        SCRIPT_LOG_ERROR("Send to node %d failed\r\n", node);
        csp_buffer_free(packet);
        csp_close(conn);
        return -1;
    }

    /* Wait for a response */
    packet = csp_read(conn, SCRIPT_REMOTE_TIMEOUT_MS);
    if (packet != NULL) {
        SCRIPT_LOG_INFO("Response from node %d: %s\r\n", node, (char *)packet->data);
        csp_buffer_free(packet);
    } else {
        SCRIPT_LOG_WARN("No response from node %d\r\n", node);
    }

    csp_close(conn);
    return 0;
}

void script_process_remote_command(csp_conn_t *conn, const char *remote_command) {
    if (!remote_command || !conn) return;

    char cmd_buffer[256];
    strncpy(cmd_buffer, remote_command, sizeof(cmd_buffer) - 1);
    cmd_buffer[sizeof(cmd_buffer) - 1] = '\0';

    char *command = strtok(cmd_buffer, " ");
    char *args = strtok(NULL, "");

    const char *response_msg = "OK";

    if (command) {
        for (int i = 0; remote_command_table[i].command_name != NULL; i++) {
            if (strcmp(command, remote_command_table[i].command_name) == 0) {
                remote_command_table[i].handler(args);
                goto send_response;
            }
        }
        response_msg = "Error: Unknown command";
    } else {
        response_msg = "Error: Invalid command format";
    }


send_response:
    csp_packet_t *response_packet = csp_buffer_get(strlen(response_msg) + 1);
    if (response_packet) {
        strcpy((char *)response_packet->data, response_msg);
        response_packet->length = strlen(response_msg) + 1;
        csp_send(conn, response_packet, 1000);
    }
}


void script_loop_local(const char *args) {
    /* Atomic check-and-set to prevent race condition */
    bool was_running = atomic_exchange(&g_loop_running, true);
    if (was_running) {
        SCRIPT_LOG_WARN("A script is already running. Use 'stop' to end it.\r\n");
        return;
    }
    
    if (!args) {
        SCRIPT_LOG_ERROR("Usage: loop <file> [delay_ms]\r\n");
        atomic_store(&g_loop_running, false);
        return;
    }

    char *args_copy = strdup(args);
    if (!args_copy) {
        SCRIPT_LOG_ERROR("Out of memory.\r\n");
        atomic_store(&g_loop_running, false);
        return;
    }

    if (csp_thread_create(task_script_looper, "Sc_Loop", SCRIPT_LOOP_TASK_STACK_SIZE, 
                          args_copy, osPriorityAboveNormal, NULL) != CSP_ERR_NONE) {
        SCRIPT_LOG_ERROR("Could not create script loop task.\r\n");
        atomic_store(&g_loop_running, false);
        free(args_copy);
    } else {
        SCRIPT_LOG_INFO("Script loop started. Use 'stop' to end.\r\n");
    }
}

void script_stop_local(const char *args) {
    (void)args; /* Unused parameter */
    
    if (atomic_exchange(&g_loop_running, false)) {
        SCRIPT_LOG_INFO("Stopping script loop...\r\n");
    } else {
        SCRIPT_LOG_WARN("No script loop is currently running.\r\n");
    }
}

#endif /* ENABLE_SCRIPT_MANAGER */
