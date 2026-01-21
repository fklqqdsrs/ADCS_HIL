#include "cli_rtable.h"

int set_address_csp(const char *address)
{

    FILE *file = fopen("routing_table.txt", "r");
    char routing_table[256] = {0};

    if (file != NULL)
    {

        char dummy[256];
        if (fgets(dummy, sizeof(dummy), file) == NULL) {
   return -1 ;
}

        if (fgets(routing_table, sizeof(routing_table), file) == NULL) {
    return -2;
}

        fclose(file);
    }

    file = fopen("routing_table.txt", "w");
    if (file == NULL)
    {
        printf("Error: Unable to open file for writing\n");
        return -1;
    }

    if (fprintf(file, "address %s\n", address) < 0)
    {
        printf("Error: Failed to write address to file\n");
        fclose(file);
        return -1;
    }

    if (routing_table[0] != '\0')
    {
        if (fprintf(file, "%s", routing_table) < 0)
        {
            printf("Error: Failed to write routing table to file\n");
            fclose(file);
            return -1;
        }
    }

    fclose(file);
    return 0;
}

int set_routing_csp(const char *routing)
{

    FILE *file = fopen("routing_table.txt", "r");
    char address[256] = {0};

    if (file != NULL)
    {

        if (fgets(address, sizeof(address), file) == NULL)
        {
            printf("Error: Failed to read address\n");
            fclose(file);
            return -1;
        }
        fclose(file);
    }

    file = fopen("routing_table.txt", "w");
    if (file == NULL)
    {
        printf("Error: Unable to open file for writing\n");
        return -1;
    }

    if (fprintf(file, "%s", address) < 0)
    {
        printf("Error: Failed to write address\n");
        fclose(file);
        return -1;
    }

    if (fprintf(file, "%s\n", routing) < 0)
    {
        printf("Error: Failed to write routing to file\n");
        fclose(file);
        return -1;
    }

    fclose(file);
    return 0;
}

int read_address(uint8_t *address)
{
    FILE *file = fopen("routing_table.txt", "r");
    if (file == NULL)
    {
        csp_log_error("Error: Unable to open file for reading: address\n");
        return -1;
    }

    char buffer[256];
    if (fgets(buffer, sizeof(buffer), file) == NULL)
    {
        fclose(file);
        return -1;
    }

    int temp;
    if (sscanf(buffer, "address %d", &temp) != 1)
    {
        fclose(file);
        return -1;
    }

    if (temp < 0 || temp > 255)
    {
        csp_log_error("Error: Address value out of range\n");
        fclose(file);
        return -1;
    }

    *address = (uint8_t)temp;

    fclose(file);
    return 0;
}

int read_routing_table(char **rtable)
{
    FILE *file = fopen("routing_table.txt", "r");
    if (file == NULL)
    {
        csp_log_error("Error: Unable to open file for reading: routing table\n");
        return -1;
    }

    char dummy[256];
    if (fgets(dummy, sizeof(dummy), file) == NULL)
    {
        csp_log_error("Error: Failed to read address\n");
        fclose(file);
        return -1;
    }

    char *buffer = (char *)malloc(256 * sizeof(char));
    if (buffer == NULL)
    {
        csp_log_error("Error: Memory allocation failed\n");
        fclose(file);
        return -1;
    }

    if (fgets(buffer, 256, file) == NULL)
    {
        csp_log_error("No routing table found or failed to read.\n");
        free(buffer);
        *rtable = NULL;
        fclose(file);
        return -1;
    }

    char *newline = strchr(buffer, '\n');
    if (newline != NULL)
    {
        *newline = '\0';
    }

    fclose(file);
    *rtable = buffer;
    return 0;
}

int reset_address()
{
    FILE *file = fopen("routing_table.txt", "r");
    if (file == NULL)
    {
        csp_log_error("Error: Unable to open file for resetting address\n");
        return -1;
    }

    char dummy[256];
    if (fgets(dummy, sizeof(dummy), file) == NULL)
    {
        csp_log_error("Error: Failed to read address\n");
        fclose(file);
        return -1;
    }

    char routing_table[256];
    if (fgets(routing_table, sizeof(routing_table), file) == NULL)
    {

        fclose(file);
        FILE *file_write = fopen("routing_table.txt", "w");
        if (file_write != NULL)
            fclose(file_write);
        printf("Address successfully reset (removed address).\n");
        return 0;
    }
    fclose(file);

    FILE *file_write = fopen("routing_table.txt", "w");
    if (file_write == NULL)
    {
        csp_log_error("Error: Unable to open file for writing\n");
        return -1;
    }

    if (fprintf(file_write, "\n%s", routing_table) < 0)
    {
        csp_log_error("Error: Failed to reset address\n");
        fclose(file_write);
        return -1;
    }

    fclose(file_write);
    printf("Address successfully reset (removed address).\n");
    return 0;
}

int reset_rtable()
{
    FILE *file = fopen("routing_table.txt", "r");
    if (file == NULL)
    {
        csp_log_error("Error: Unable to open file for resetting routing table\n");
        return -1;
    }

    char address[256];
    if (fgets(address, sizeof(address), file) == NULL)
    {

        fclose(file);
        FILE *file_write = fopen("routing_table.txt", "w");
        fclose(file_write);
        printf("Routing table successfully reset (removed routing table).\n");
        return 0;
    }

    FILE *file_write = fopen("routing_table.txt", "w");
    if (file_write == NULL)
    {
        csp_log_error("Error: Unable to open file for writing\n");
        fclose(file);
        return -1;
    }

    if (fprintf(file_write, "%s", address) < 0)
    {
        csp_log_error("Error: Failed to reset routing table\n");
        fclose(file_write);
        return -1;
    }

    fclose(file_write);
    fclose(file);
    printf("Routing table successfully reset (removed routing table).\n");
    return 0;
}

void csp_set_address_command(const char *args)
{
    if (args == NULL || strlen(args) == 0)
    {
        csp_log_error("Usage: set_address <node>\n");
        return;
    }
    int result = set_address_csp(args);
    if (result == 0)
    {
        printf("Address csp successfully updated.\n");
    }
    else
    {
        csp_log_error("Failed to update Address csp.\n");
    }
}

void csp_get_address_command(const char *args)
{
    uint8_t address_csp;

    int result = read_address(&address_csp);

    if (result == 0)
    {
        printf("Address csp: %u\n", address_csp);
    }
    else
    {
        csp_log_error("Failed to retrieve Address csp.\n");
    }
}

void csp_reset_address_command(const char *args)
{
    int result = reset_address();
    if (result == 0)
    {
        printf("reset address csp successfully updated.\n");
    }
    else
    {
        csp_log_error("Failed to reset address csp.\n");
    }
}

void csp_set_rtable_command(const char *args)
{
    if (args == NULL || strlen(args) == 0)
    {
        csp_log_error("Usage: set_rtable <rtable>\n");
        csp_log_error("Please provide a valid routing table.\n");
        return;
    }
    int result = set_routing_csp(args);
    if (result == 0)
    {
        printf("Routing table successfully updated.\n");
    }
    else
    {
        csp_log_error("Failed to update routing table.\n");
    }
}

void csp_get_rtable_command(const char *args)
{
    char *rtable = NULL;

    int result = read_routing_table(&rtable);

    if (result == 0)
    {
        printf("Routing Table:\n%s\n", rtable);
        free(rtable);
    }
    else
    {
        csp_log_error("Failed to retrieve routing table.\n");
    }
}

void csp_reset_rtable_command(const char *args)
{
    int result = reset_rtable();
    if (result == 0)
    {
        printf("reset routing table successfully updated.\n");
    }
    else
    {
        csp_log_error("Failed to reset routing table.\n");
    }
}