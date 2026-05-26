#include "app_modules_config.h"
#if ENABLE_SCRIPT_MANAGER

/*
 * File:    script_cli.c
 * Author: Chaturong
 * Description: Command line interface for script scheduling and management
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "fatfs.h"

#include "script_scheduler.h"
#include "script_manager.h"

#include "kservice.h"
#include "finsh.h"     /* provides FSymTab begin/end and FINSH_NEXT_SYSCALL */

/* ===== Configuration Macros ===== */
#ifndef CLI_MAX_LINE_LEN
#define CLI_MAX_LINE_LEN                 256
#endif

#ifndef CLI_MAX_CMD_LEN
#define CLI_MAX_CMD_LEN                  128
#endif

/* ===== Path validation ===== */
static int validate_filename(const char *filename) {
    if (!filename || strlen(filename) == 0) return -1;
    
    /* Prevent path traversal attacks */
    if (strstr(filename, "..") != NULL) return -1;
    if (strstr(filename, "/") != NULL) return -1;
    if (strstr(filename, "\\") != NULL) return -1;
    
    /* Validate filename length */
    if (strlen(filename) >= CLI_MAX_CMD_LEN) return -1;
    
    return 0;
}



/* ---------------- Scheduler commands ---------------- */

static int __sched_add_abs_cmd(int argc, char **argv) {
    if (argc != 3) { printf("Usage: sched_add_abs <utc_s> <file>\r\n"); return -1; }
    if (validate_filename(argv[2]) != 0) { printf("ERR: Invalid filename\r\n"); return -1; }
    uint32_t utc_s = (uint32_t)strtoul(argv[1], NULL, 10);
    int id = script_schedule_at_abs(utc_s, argv[2]);
    printf((id >= 0) ? "OK id=%d\r\n" : "ERR\r\n", id);
    return (id >= 0) ? 0 : -1;
}

static int __sched_after_cmd(int argc, char **argv) {
    if (argc != 3) { printf("Usage: sched_after <ms> <file>\r\n"); return -1; }
    if (validate_filename(argv[2]) != 0) { printf("ERR: Invalid filename\r\n"); return -1; }
    uint32_t ms = (uint32_t)strtoul(argv[1], NULL, 10);
    int id = script_schedule_after(ms, argv[2]);
    printf((id >= 0) ? "OK id=%d\r\n" : "ERR\r\n", id);
    return (id >= 0) ? 0 : -1;
}

static int __sched_daily_cmd(int argc, char **argv) {
    if (argc != 3) { printf("Usage: sched_daily <HH:MM> <file>\r\n"); return -1; }
    if (validate_filename(argv[2]) != 0) { printf("ERR: Invalid filename\r\n"); return -1; }
    int id = script_schedule_daily(argv[1], argv[2]);
    printf((id >= 0) ? "OK id=%d\r\n" : "ERR\r\n", id);
    return (id >= 0) ? 0 : -1;
}

static int __sched_every_cmd(int argc, char **argv) {
    if (argc != 3) { printf("Usage: sched_every <ms> <file>\r\n"); return -1; }
    if (validate_filename(argv[2]) != 0) { printf("ERR: Invalid filename\r\n"); return -1; }
    uint32_t ms = (uint32_t)strtoul(argv[1], NULL, 10);
    int id = script_schedule_every(ms, argv[2]);
    printf((id >= 0) ? "OK id=%d\r\n" : "ERR\r\n", id);
    return (id >= 0) ? 0 : -1;
}

static int __sched_set_cmd(int argc, char **argv) {
    if (argc != 4) { printf("Usage: sched_set <id> <enable|policy|remain> <val>\r\n"); return -1; }
    int id = atoi(argv[1]);
    const char *field = argv[2];
    const char *val   = argv[3];
    int rc = -1;
    if (strcmp(field, "enable") == 0) {
        rc = script_schedule_set_enabled(id, (strcmp(val, "1") == 0));
    } else if (strcmp(field, "policy") == 0) {
        missed_policy_t p = (strcmp(val, "catch") == 0) ? MISSED_CATCH_UP_ONCE : MISSED_SKIP;
        rc = script_schedule_set_policy(id, p);
    } else if (strcmp(field, "remain") == 0) {
        rc = script_schedule_set_remain(id, (uint16_t)strtoul(val, NULL, 10));
    }
    printf((rc == 0) ? "OK\r\n" : "ERR\r\n");
    return (rc == 0) ? 0 : -1;
}

static int __sched_del_cmd(int argc, char **argv) {
    if (argc != 2) { printf("Usage: sched_del <id>\r\n"); return -1; }
    int id = atoi(argv[1]);
    int rc = script_schedule_delete(id);
    printf((rc == 0) ? "OK\r\n" : "ERR\r\n");
    return (rc == 0) ? 0 : -1;
}

static int __sched_list_cmd(int argc, char **argv) {
    (void)argc; (void)argv;
    script_schedule_list();
    return 0;
}

static int __sched_save_cmd(int argc, char **argv) {
    (void)argc; (void)argv;
    (void)script_persist_save();
    printf("OK\r\n");
    return 0;
}

static int __sched_load_cmd(int argc, char **argv) {
    (void)argc; (void)argv;
    int rc = script_persist_load();
    printf((rc==0)?"OK\r\n":"ERR\r\n");
    return (rc==0)?0:-1;
}

static int __sched_clear_cmd(int argc, char **argv) {
    (void)argc; (void)argv;
    script_schedule_clear();
    printf("OK\r\n");
    return 0;
}

static int __sched_stop_cmd(int argc, char **argv) {
    if (argc != 2) { printf("Usage: sched_stop <id>\r\n"); return -1; }
    int id = atoi(argv[1]);
    int rc = script_schedule_set_enabled(id, 0);   /* disable */
    printf((rc == 0) ? "OK\r\n" : "ERR\r\n");
    return (rc == 0) ? 0 : -1;
}


/* ---------------- Local/Remote run commands ---------------- */

static int __run_cmd(int argc, char **argv) {
    if (argc != 2) { printf("Usage: run <file>\r\n"); return -1; }
    if (validate_filename(argv[1]) != 0) { printf("ERR: Invalid filename\r\n"); return -1; }
    return script_run_local(argv[1]), 0;
}

static int __loop_cmd(int argc, char **argv) {
    if (argc < 2 || argc > 3) { printf("Usage: loop <file> [delay_ms]\r\n"); return -1; }
    if (validate_filename(argv[1]) != 0) { printf("ERR: Invalid filename\r\n"); return -1; }
    const char *file = argv[1];
    const char *delay = (argc == 3) ? argv[2] : NULL;
    char buf[CLI_MAX_CMD_LEN];
    if (delay) snprintf(buf, sizeof(buf), "%s %s", file, delay);
    else       snprintf(buf, sizeof(buf), "%s", file);
    return script_loop_local(buf), 0;
}

static int __stop_cmd(int argc, char **argv) {
    (void)argc; (void)argv;
    return script_stop_local(NULL), 0;
}

static int __run_remote_cmd(int argc, char **argv) {
    if (argc != 3) { printf("Usage: run_remote <node> <script.txt>\r\n"); return -1; }
    if (validate_filename(argv[2]) != 0) { printf("ERR: Invalid filename\r\n"); return -1; }
    char cmd[CLI_MAX_CMD_LEN]; 
    snprintf(cmd, sizeof(cmd), "R %s", argv[2]);
    script_send_remote_command(atoi(argv[1]), cmd);
    return 0;
}

static int __loop_remote_cmd(int argc, char **argv) {
    if (argc < 3 || argc > 4) { printf("Usage: loop_remote <node> <script.txt> [delay_ms]\r\n"); return -1; }
    if (validate_filename(argv[2]) != 0) { printf("ERR: Invalid filename\r\n"); return -1; }
    char cmd[CLI_MAX_CMD_LEN];
    if (argc == 4) snprintf(cmd, sizeof(cmd), "L %s %s", argv[2], argv[3]);
    else           snprintf(cmd, sizeof(cmd), "L %s",    argv[2]);
    script_send_remote_command(atoi(argv[1]), cmd);
    return 0;
}

static int __stop_remote_cmd(int argc, char **argv) {
    if (argc != 2) { printf("Usage: stop_remote <node>\r\n"); return -1; }
    script_send_remote_command(atoi(argv[1]), "S");
    return 0;
}

/* ---------------- File operations ---------------- */

static int __show_cmd(int argc, char **argv) {
    if (argc != 2) { printf("Usage: show <filename>\r\n"); return -1; }
    if (validate_filename(argv[1]) != 0) { printf("ERR: Invalid filename\r\n"); return -1; }
    const char *filename = argv[1];
    FIL file;
    if (f_open(&file, filename, FA_READ) != FR_OK) {
        printf("Error: Could not open '%s'\r\n", filename);
        return -1;
    }
    char line[CLI_MAX_LINE_LEN];
    printf("--- Start of %s ---\r\n", filename);
    while (f_gets(line, sizeof(line), &file)) { printf("%s", line); }
    printf("--- End of %s ---\r\n", filename);
    f_close(&file);
    return 0;
}

static int __save_cmd(int argc, char **argv) {
    if (argc < 4) { printf("Usage: save <file> <w|a> <command...>\r\n"); return -1; }
    if (validate_filename(argv[1]) != 0) { printf("ERR: Invalid filename\r\n"); return -1; }
    const char *filename = argv[1];
    const char *mode_str = argv[2];
    
    /* Join argv[3..] to one line using snprintf (safe, no buffer overflow) */
    char line_to_save[CLI_MAX_LINE_LEN] = {0};
    size_t remaining = sizeof(line_to_save);
    char *ptr = line_to_save;
    
    for (int i = 3; i < argc && remaining > 0; ++i) {
        int len = snprintf(ptr, remaining, "%s%s", (i == 3) ? "" : " ", argv[i]);
        if (len < 0 || (size_t)len >= remaining) {
            break; /* Buffer full or error */
        }
        ptr += len;
        remaining -= (size_t)len;
    }
    
    BYTE mode = (strcmp(mode_str, "w") == 0) ? (FA_CREATE_ALWAYS | FA_WRITE) : (FA_OPEN_APPEND | FA_WRITE);
    FIL file;
    if (f_open(&file, filename, mode) != FR_OK) { printf("Error opening '%s'\r\n", filename); return -1; }
    f_printf(&file, "%s\n", line_to_save);
    f_close(&file);
    printf("Saved to '%s'\r\n", filename);
    return 0;
}

static int __clean_cmd(int argc, char **argv) {
    if (argc != 2) { printf("Usage: clean <filename>\r\n"); return -1; }
    if (validate_filename(argv[1]) != 0) { printf("ERR: Invalid filename\r\n"); return -1; }
    FIL file;
    if (f_open(&file, argv[1], FA_CREATE_ALWAYS) == FR_OK) {
        f_close(&file);
        printf("Cleaned '%s'\r\n", argv[1]);
        return 0;
    }
    printf("Error cleaning '%s'\r\n", argv[1]);
    return -1;
}

static int __delete_cmd(int argc, char **argv) {
    if (argc != 2) { printf("Usage: delete <filename>\r\n"); return -1; }
    if (validate_filename(argv[1]) != 0) { printf("ERR: Invalid filename\r\n"); return -1; }
    if (f_unlink(argv[1]) == FR_OK) { printf("Deleted '%s'\r\n", argv[1]); return 0; }
    printf("Error deleting '%s'\r\n", argv[1]); return -1;
}


TSC_CMD_EXPORT_ALIAS(__show_cmd,        show,        "Show content of a script file");
TSC_CMD_EXPORT_ALIAS(__save_cmd,        save,        "Append/overwrite a line into a script file");
TSC_CMD_EXPORT_ALIAS(__clean_cmd,       clean,       "Erase a script file");
TSC_CMD_EXPORT_ALIAS(__delete_cmd,      delete,      "Delete a script file");

TSC_CMD_EXPORT_ALIAS(__run_cmd,         run,         "Execute a script once");
TSC_CMD_EXPORT_ALIAS(__loop_cmd,        loop,        "Loop a script file");
TSC_CMD_EXPORT_ALIAS(__stop_cmd,        stop,        "Stop current script loop");

TSC_CMD_EXPORT_ALIAS(__run_remote_cmd,  run_remote,  "Execute script on a remote node");
TSC_CMD_EXPORT_ALIAS(__loop_remote_cmd, loop_remote, "Loop script on a remote node");
TSC_CMD_EXPORT_ALIAS(__stop_remote_cmd, stop_remote, "Stop remote script loop");

TSC_CMD_EXPORT_ALIAS(__sched_add_abs_cmd, sched_add_abs, "Schedule at absolute UTC seconds");
TSC_CMD_EXPORT_ALIAS(__sched_after_cmd,   sched_after,   "Schedule once after ms");
TSC_CMD_EXPORT_ALIAS(__sched_daily_cmd,   sched_daily,   "Schedule daily at HH:MM");
TSC_CMD_EXPORT_ALIAS(__sched_every_cmd,   sched_every,   "Schedule periodic every ms");
TSC_CMD_EXPORT_ALIAS(__sched_set_cmd,     sched_set,     "Set schedule field");
TSC_CMD_EXPORT_ALIAS(__sched_list_cmd,    sched_list,    "List schedules");
TSC_CMD_EXPORT_ALIAS(__sched_del_cmd,     sched_del,     "Delete schedule by id");
TSC_CMD_EXPORT_ALIAS(__sched_save_cmd,    sched_save,    "Persist schedules");
TSC_CMD_EXPORT_ALIAS(__sched_load_cmd,    sched_load,    "Load schedules");
TSC_CMD_EXPORT_ALIAS(__sched_clear_cmd,   sched_clear,   "Clear schedules");
TSC_CMD_EXPORT_ALIAS(__sched_stop_cmd,  sched_stop,  "Disable a scheduled entry by id");


#endif /* ENABLE_SCRIPT_MANAGER */
