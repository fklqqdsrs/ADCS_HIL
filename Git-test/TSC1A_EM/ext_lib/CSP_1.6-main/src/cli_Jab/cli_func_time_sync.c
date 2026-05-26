#include "cli_csp.h"

void csp_set_timesync_command(const char *args) {
    uint8_t node;
    uint32_t time_sync;
    uint32_t timeout;

    char *args_copy = strdup(args);
    char *token = strtok(args_copy, " ");

    if (token != NULL) node = (uint8_t)atoi(token);
    token = strtok(NULL, " ");
    if (token != NULL) time_sync = (uint32_t)strtoul(token, NULL, 10);
    token = strtok(NULL, " ");
    if (token != NULL) timeout = (uint32_t)strtoul(token, NULL, 10);

    if (token != NULL) {
        printf("Running set_time_sync: node=%d, timesync=%lu, timeout=%lu\n", node, (unsigned long)time_sync, (unsigned long)timeout);
        csp_set_time_sync(node, time_sync, timeout);
    } else {
        printf("Usage: set_time_sync <node> <time_sync> <timeout>\n");
    }

    free(args_copy);
}

void csp_timesync_broadcast_command(const char *args) {
    uint32_t time_sync;
    uint32_t timeout;

    char *args_copy = strdup(args);
    char *token = strtok(args_copy, " ");

    if (token != NULL) time_sync = (uint32_t)strtoul(token, NULL, 10);
    token = strtok(NULL, " ");
    if (token != NULL) timeout = (uint32_t)strtoul(token, NULL, 10);

    if (token != NULL) {
        printf("Running set_time_broadcast: timesync=%lu, timeout=%lu\n", (unsigned long)time_sync, (unsigned long)timeout);
        csp_time_sync_broadcast(time_sync, timeout);
    } else {
        printf("Usage: set_time_broadcast <time_sync> <timeout>\n");
    }

    free(args_copy);
}

void csp_get_timesync_command(const char *args) {
    uint8_t node;
    uint32_t timeout;

    char *args_copy = strdup(args);
    char *token = strtok(args_copy, " ");

    if (token != NULL) node = (uint8_t)atoi(token);
    token = strtok(NULL, " ");
    if (token != NULL) timeout = (uint32_t)strtoul(token, NULL, 10);

    if (token != NULL) {
        printf("Running get_time_sync: node=%d, timeout=%lu\n", node, (unsigned long)timeout);
        uint32_t get_time_sync;
        csp_get_time_sync(node, timeout, &get_time_sync);
    } else {
        printf("Usage: get_time_sync <node> <timeout>\n");
    }

    free(args_copy);
}
