#include "cli_csp.h"

void send_chicken_command(const char *args)
{
    // node prio dport timeout opts data[]
    uint8_t node = 0, prio = 0, opts = 0;
    uint16_t dport = 0;
    uint32_t timeout = 0;
    uint8_t data[255] = {0};
    uint16_t data_length = 0;

    if (args)
    {
        char *args_copy = strdup(args);
        char *token = strtok(args_copy, " ");

        if (token != NULL)
        {
            node = (uint8_t)atoi(token);
            token = strtok(NULL, " ");
        }
        if (token != NULL)
        {
            prio = (uint8_t)atoi(token);
            token = strtok(NULL, " ");
        }
        if (token != NULL)
        {
            dport = (uint16_t)atoi(token);
            token = strtok(NULL, " ");
        }
        if (token != NULL)
        {
            timeout = (uint32_t)atoi(token);
            token = strtok(NULL, " ");
        }
        if (token != NULL)
        {
            opts = (uint8_t)atoi(token);
            token = strtok(NULL, " ");
        }
        if (token != NULL)
        {
            strncpy((char *)data, token, sizeof(data) - 1);
            data[sizeof(data) - 1] = '\0'; // Ensure null termination
            data_length = (uint16_t)strlen((char *)data);
        }

        free(args_copy);

        if (data_length > 0)
        {
            int result = send_chicken(node, prio, dport, timeout, opts, data, data_length);
            printf("Sending data to node=%d, prio=%d, dport=%d, timeout=%u, opts=%d, data=\"%s\", Data length: %u, result=%d\n",
                   node, prio, dport, timeout, opts, data, data_length, result);
        }
        else
        {
            printf("Invalid data or insufficient arguments provided.\n");
        }
    }
    else
    {
        printf("Usage: send <node> <prio> <dport> <timeout> <opts> <data>\n");
    }
}


void request_chicken_command(const char *args)
{
    uint8_t node = 0, prio = 0, opts = 0;
    uint16_t dport = 0;
    uint32_t timeout = 0;
    char send_data[255] = {0};
    uint8_t recv_data[255] = {0};
    size_t send_data_length = 0;
    uint16_t recv_data_length = sizeof(recv_data);

    if (args)
    {
        char *args_copy = strdup(args);
        char *token = strtok(args_copy, " ");

        if (token != NULL)
        {
            node = (uint8_t)atoi(token);
            token = strtok(NULL, " ");
        }
        if (token != NULL)
        {
            prio = (uint8_t)atoi(token);
            token = strtok(NULL, " ");
        }
        if (token != NULL)
        {
            dport = (uint16_t)atoi(token);
            token = strtok(NULL, " ");
        }
        if (token != NULL)
        {
            timeout = (uint32_t)atoi(token);
            token = strtok(NULL, " ");
        }
        if (token != NULL)
        {
            opts = (uint8_t)atoi(token);
            token = strtok(NULL, " ");
        }
        if (token != NULL)
        {
            strncpy(send_data, token, sizeof(send_data) - 1);
            send_data[sizeof(send_data) - 1] = '\0'; // Ensure null termination
            send_data_length = strlen(send_data);
        }

        free(args_copy);

        if (send_data_length > 0)
        {
            int result = request_chicken(node, prio, dport, timeout, opts,
                                         (uint8_t *)send_data, send_data_length,
                                         recv_data, &recv_data_length);

            printf("Sending request to node=%d, prio=%d, dport=%d, timeout=%u, opts=%d, send_data=\"%s\" (length=%zu), result=%d\n",
                   node, prio, dport, timeout, opts, send_data, send_data_length, result);

            if (result == 0)
            {
                recv_data[recv_data_length] = '\0'; // Ensure null termination
                printf("Response received: \"%s\" (length=%u)\n", recv_data, recv_data_length);
            }
            else
            {
                memset(recv_data, 0, sizeof(recv_data)); // Reset recv_data
                recv_data_length = 0;
            }
        }
        else
        {
            printf("Invalid send_data or insufficient arguments provided.\n");
        }
    }
    else
    {
        printf("Usage: request <node> <prio> <dport> <timeout> <opts> <send_data>\n");
    }
}


void send_chicken_hex_command(const char *args)
{
    uint8_t node = 0, prio = 0, opts = 0;
    uint16_t dport = 0;
    uint32_t timeout = 0;
    uint8_t data[255] = {0};
    uint16_t data_length = 0;

    if (args)
    {
        char *args_copy = strdup(args);
        char *token = strtok(args_copy, " ");

        if (token != NULL)
        {
            node = (uint8_t)atoi(token);
            token = strtok(NULL, " ");
        }
        if (token != NULL)
        {
            prio = (uint8_t)atoi(token);
            token = strtok(NULL, " ");
        }
        if (token != NULL)
        {
            dport = (uint16_t)atoi(token);
            token = strtok(NULL, " ");
        }
        if (token != NULL)
        {
            timeout = (uint32_t)atoi(token);
            token = strtok(NULL, " ");
        }
        if (token != NULL)
        {
            opts = (uint8_t)atoi(token);
            token = strtok(NULL, " ");
        }
        if (token != NULL)
        {
            char *hex_ptr = token;
            while (*hex_ptr && *(hex_ptr + 1) && data_length < sizeof(data))
            {
                char byte_str[3] = {hex_ptr[0], hex_ptr[1], '\0'};
                data[data_length++] = (uint8_t)strtol(byte_str, NULL, 16);
                hex_ptr += 2;
            }
        }

        free(args_copy);

        if (data_length > 0)
        {
            int result = send_chicken(node, prio, dport, timeout, opts, data, data_length);

            printf("Sending data to node=%d, prio=%d, dport=%d, timeout=%u, opts=%d, data=[",
                   node, prio, dport, timeout, opts);
            for (int i = 0; i < data_length; i++)
            {
                printf("%02X", data[i]);
                if (i < data_length - 1)
                    printf(" ");
            }
            printf("], Data length: %u, result=%d\n", data_length, result);
        }
        else
        {
            printf("Invalid or empty hex data.\n");
        }
    }
    else
    {
        printf("Usage: send_hex <node> <prio> <dport> <timeout> <opts> <hex_data>\n");
    }
}


void request_chicken_hex_command(const char *args)
{
    uint8_t node = 0, prio = 0, opts = 0;
    uint16_t dport = 0;
    uint32_t timeout = 0;
    uint8_t send_data[255] = {0};
    uint16_t send_data_length = 0;

    uint8_t recv_data[255] = {0};
    uint16_t recv_data_length = sizeof(recv_data);

    if (args)
    {
        char *args_copy = strdup(args);
        char *token = strtok(args_copy, " ");

        if (token != NULL)
        {
            node = (uint8_t)atoi(token);
            token = strtok(NULL, " ");
        }
        if (token != NULL)
        {
            prio = (uint8_t)atoi(token);
            token = strtok(NULL, " ");
        }
        if (token != NULL)
        {
            dport = (uint16_t)atoi(token);
            token = strtok(NULL, " ");
        }
        if (token != NULL)
        {
            timeout = (uint32_t)atoi(token);
            token = strtok(NULL, " ");
        }
        if (token != NULL)
        {
            opts = (uint8_t)atoi(token);
            token = strtok(NULL, " ");
        }
        if (token != NULL)
        {
            char *hex_ptr = token;
            while (*hex_ptr && *(hex_ptr + 1) && send_data_length < sizeof(send_data))
            {
                char byte_str[3] = {hex_ptr[0], hex_ptr[1], '\0'};
                send_data[send_data_length++] = (uint8_t)strtol(byte_str, NULL, 16);
                hex_ptr += 2;
            }
        }

        free(args_copy);

        if (send_data_length > 0)
        {
            int result = request_chicken(node, prio, dport, timeout, opts,
                                         send_data, send_data_length,
                                         recv_data, &recv_data_length);

            printf("Sending request to node=%d, prio=%d, dport=%d, timeout=%u, opts=%d, send_data=[",
                   node, prio, dport, timeout, opts);
            for (int i = 0; i < send_data_length; i++)
            {
                printf("%02X", send_data[i]);
                if (i < send_data_length - 1)
                    printf(" ");
            }
            printf("] (length=%u), result=%d\n", send_data_length, result);

            if (result == 0)
            {
                printf("Response received: [");
                for (int i = 0; i < recv_data_length; i++)
                {
                    printf("%02X", recv_data[i]);
                    if (i < recv_data_length - 1)
                        printf(" ");
                }
                printf("] (length=%u)\n", recv_data_length);
            }
            else
            {
                printf("Request failed with result=%d\n", result);
            }
        }
        else
        {
            printf("Invalid or empty hex data.\n");
        }
    }
    else
    {
        printf("Usage: request_hex <node> <prio> <dport> <timeout> <opts> <hex_send_data>\n");
    }
}
