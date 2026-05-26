#include "cli_csp.h"


void csp_ping_command(const char *args)
{
    uint8_t node = 0;
    uint32_t timeout = 0;
    unsigned int size = 0;
    uint8_t opts = 0;

    char *token;
    char *args_copy = strdup(args);
    token = strtok(args_copy, " ");

    if (token != NULL) {
        node = (uint8_t)atoi(token);
        token = strtok(NULL, " ");
    }
    if (token != NULL) {
        timeout = (uint32_t)atoi(token);
        token = strtok(NULL, " ");
    }
    if (token != NULL) {
        size = (unsigned int)atoi(token);
        token = strtok(NULL, " ");
    }
    if (token != NULL) {
        opts = (uint8_t)atoi(token);
    }

    if (node > 0)
    {
        printf("Running ping: node=%d, timeout=%lu, size=%u, opts=%d\n", node, timeout, size, opts);
        int result = csp_ping(node, timeout, size, opts);
        csp_log_info("Ping address: %u, result %d [mS]", node, result);
    }
    else
    {
        printf("Usage: ping <node> <timeout> <size> <opts>\n");
    }

    free(args_copy);
}





void csp_ping_all_command(const char *args)
{
    uint32_t timeout = 0;
    unsigned int size = 0;
    uint8_t opts = 0;

    if (args) {
        char *args_copy = strdup(args);
        char *token = strtok(args_copy, " ");

        if (token != NULL) {
            timeout = (uint32_t)strtol(token, NULL, 10);
            token = strtok(NULL, " ");
        }
        if (token != NULL) {
            size = (unsigned int)strtol(token, NULL, 10);
            token = strtok(NULL, " ");
        }
        if (token != NULL) {
            opts = (uint8_t)strtol(token, NULL, 10);
        }

        for (uint8_t node = 1; node <= 31; node++)
        {
            printf("Running ping_all: node=%d, timeout=%lu, size=%u, opts=%d\n", node, timeout, size, opts);
            int result = csp_ping(node, timeout, size, opts);
            csp_log_info("Ping address: %u, result %d [mS]", node, result);
        }

        free(args_copy);
    }
    else
    {
        printf("Usage: ping_all <timeout> <size> <opts>\n");
    }
}


void csp_ping_noreply_command(const char *args)
{
    uint8_t node = 0;

    if (args) {
        char *args_copy = strdup(args);
        char *token = strtok(args_copy, " ");

        if (token != NULL) {
            node = (uint8_t)strtol(token, NULL, 10);
        }

        if (node > 0)
        {
            printf("Running ping_noreply: node=%d\n", node);
            csp_ping_noreply(node);
        }
        else
        {
            printf("Usage: ping_noreply <node>\n");
        }

        free(args_copy);
    }
    else
    {
        printf("Usage: ping_noreply <node>\n");
    }
}

void csp_reboot_command(const char *args)
{
    uint8_t node;
    if (args){
       char *args_copy = strdup(args);
        char *token = strtok(args_copy, " ");

        if (token != NULL) {
            node = (uint8_t)strtol(token, NULL, 10);
        }

        if (node > 0)
        {
            printf("Running reboot: node=%d\n", node);
            csp_reboot(node);
        }
        else
        {
            printf("Usage: reboot <node>\n");
        }

        free(args_copy);
    }
}

void csp_shutdown_command(const char *args)
{
    uint8_t node = 0;

    if (args) {
        char *args_copy = strdup(args);
        char *token = strtok(args_copy, " ");

        if (token != NULL) {
            node = (uint8_t)strtol(token, NULL, 10);
        }

        if (node > 0)
        {
            printf("Running shutdown: node=%d\n", node);
            csp_shutdown(node);
        }
        else
        {
            printf("Usage: shutdown <node>\n");
        }

        free(args_copy);
    }
    else
    {
        printf("Usage: shutdown <node>\n");
    }
}

void csp_ps_command(const char *args)
{
    uint8_t node = 0;
    uint32_t timeout = 0;

    if (args) {
        char *args_copy = strdup(args);
        char *token = strtok(args_copy, " ");

        if (token != NULL) {
            node = (uint8_t)strtol(token, NULL, 10);
            token = strtok(NULL, " ");
        }

        if (token != NULL) {
            timeout = (uint32_t)strtoul(token, NULL, 10);
        }

        if (node > 0 && timeout > 0)
        {
            printf("Running ps: node=%d, timeout=%lu\n", node, timeout);
            csp_ps(node, timeout);
        }
        else
        {
            printf("Usage: ps <node> <timeout>\n");
        }

        free(args_copy);
    }
    else
    {
        printf("Usage: ps <node> <timeout>\n");
    }
}

void csp_memfree_command(const char *args)
{
    uint8_t node = 0;
    uint32_t timeout = 0;

    if (args) {
        char *args_copy = strdup(args);
        char *token = strtok(args_copy, " ");

        if (token != NULL) {
            node = (uint8_t)strtol(token, NULL, 10);
            token = strtok(NULL, " ");
        }

        if (token != NULL) {
            timeout = (uint32_t)strtoul(token, NULL, 10);
        }

        if (node > 0 && timeout > 0)
        {
            printf("Running memfree: node=%d, timeout=%lu\n", node, timeout);
            csp_memfree(node, timeout);
        }
        else
        {
            printf("Usage: memfree <node> <timeout>\n");
        }

        free(args_copy);
    }
    else
    {
        printf("Usage: memfree <node> <timeout>\n");
    }
}

void csp_buf_free_command(const char *args)
{
    uint8_t node = 0;
    uint32_t timeout = 0;

    if (args) {
        char *args_copy = strdup(args);
        char *token = strtok(args_copy, " ");

        if (token != NULL) {
            node = (uint8_t)strtol(token, NULL, 10); 
            token = strtok(NULL, " ");
        }

        if (token != NULL) {
            timeout = (uint32_t)strtoul(token, NULL, 10);
        }

        if (node > 0 && timeout > 0) 
        {
            printf("Running buf_free: node=%d, timeout=%lu\n", node, timeout);
            csp_buf_free(node, timeout);
        }
        else
        {
            printf("Usage: buf_free <node> <timeout>\n");
        }

        free(args_copy);
    }
    else
    {
        printf("Usage: buf_free <node> <timeout>\n");
    }
}

void csp_uptime_command(const char *args)
{
    uint8_t node = 0;
    uint32_t timeout = 0;

    if (args) {
        char *args_copy = strdup(args); 
        char *token = strtok(args_copy, " ");

        if (token != NULL) {
            node = (uint8_t)strtol(token, NULL, 10); 
            token = strtok(NULL, " "); 
        }

        if (token != NULL) {
            timeout = (uint32_t)strtoul(token, NULL, 10);
        }

        if (node > 0 && timeout > 0) 
        {
            printf("Running uptime: node=%d, timeout=%lu\n", node, timeout);
            csp_uptime(node, timeout);
        }
        else
        {
            printf("Usage: uptime <node> <timeout>\n");
        }

        free(args_copy);
    }
    else
    {
        printf("Usage: uptime <node> <timeout>\n");
    }
}

void csp_print_connection_table_command(const char *args)
{
    printf("Connection table\r\n");
    csp_conn_print_table();
}

void csp_print_interface_command(const char *args)
{
    printf("Interfaces\r\n");
    csp_route_print_interfaces();
}

void csp_print_rtable_command(const char *args)
{
    printf("Route table\r\n");
    csp_route_print_table();
}
