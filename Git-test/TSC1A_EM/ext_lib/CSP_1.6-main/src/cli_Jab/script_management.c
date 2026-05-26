#include "script_management.h"

CommandManager manager_script = {NULL, 0};

void add_command(CommandManager *manager, const char *name, void (*execute)(const char *args))
{
    manager->count++;
    manager->commands = realloc(manager->commands, manager->count * sizeof(Command));
    if (manager->commands == NULL)
    {
        printf("Error: Memory allocation failed!\n");
        exit(1);
    }
    manager->commands[manager->count - 1].command = name;
    manager->commands[manager->count - 1].execute = execute;
}

void remove_command(CommandManager *manager, const char *name)
{
    int found = 0;
    for (int i = 0; i < manager->count; i++)
    {
        if (strcmp(manager->commands[i].command, name) == 0)
        {
            found = 1;

            for (int j = i; j < manager->count - 1; j++)
            {
                manager->commands[j] = manager->commands[j + 1];
            }
            manager->count--;
            manager->commands = realloc(manager->commands, manager->count * sizeof(Command)); // ลดขนาดของ array
            if (manager->count > 0 && manager->commands == NULL)
            {
                printf("Error: Memory reallocation failed!\n");
                exit(1);
            }
            break;
        }
    }
    if (!found)
    {
        printf("Command not found: %s\n", name);
    }
}

void list_commands(CommandManager *manager)
{
    if (manager->count == 0)
    {
        printf("No commands available.\n");
        return;
    }

    printf("=====================================\n");
    printf("          Available Commands         \n");
    printf("=====================================\n");

    for (int i = 0; i < manager->count; i++)
    {
        printf("| %2d. %-25s |\n", i + 1, manager->commands[i].command);
    }

    printf("=====================================\n");
}

void execute_command_script(CommandManager *manager, const char *command, const char *args)
{
    if (strcmp(command, "list_commands") == 0)
    {
        list_commands(manager);
        return;
    }

    for (int i = 0; i < manager->count; i++)
    {
        if (strcmp(command, manager->commands[i].command) == 0)
        {
            manager->commands[i].execute(args);
            return;
        }
    }

    printf("Unknown command: %s\n", command);
}

void list_commands_print(const char *args){
list_commands(&manager_script);
}

void initiate_command_script()
{
    add_command(&manager_script, "ping", csp_ping_command);
    add_command(&manager_script,"ping_all",csp_ping_all_command);
    add_command(&manager_script, "ping_noreply", csp_ping_noreply_command);
    add_command(&manager_script, "ps", csp_ps_command);
    add_command(&manager_script, "memfree", csp_memfree_command);
    add_command(&manager_script, "buf_free", csp_buf_free_command);
    add_command(&manager_script, "reboot", csp_reboot_command);
    add_command(&manager_script, "shutdown", csp_shutdown_command);
    add_command(&manager_script, "uptime", csp_uptime_command);
    add_command(&manager_script, "conn_table", csp_print_connection_table_command);
    add_command(&manager_script, "interface", csp_print_interface_command);
    add_command(&manager_script, "rtable", csp_print_rtable_command);
    add_command(&manager_script, "help_command_csp", help_command_csp);

    add_command(&manager_script, "set_view", csp_set_rpm_view_command);
    add_command(&manager_script, "get_view", csp_get_rpm_view_command);
    add_command(&manager_script, "get_imu", csp_get_imu_command);
    add_command(&manager_script, "get_gps", csp_get_gps_command);
    add_command(&manager_script, "get_mtm", csp_get_mtm_command);
    add_command(&manager_script, "help_command_adcs", help_command_adcs);

    add_command(&manager_script, "set_ssdv", csp_set_ssdv_command);
    add_command(&manager_script, "reset_ssdv", csp_reset_ssdv_command);
    add_command(&manager_script, "check_ssdv", csp_check_ssdv_command);
    add_command(&manager_script, "shutdown_ssdv", csp_shutdown_ssdv_command);
    add_command(&manager_script, "help_command_ssdv", help_command_ssdv);

    add_command(&manager_script, "set_time_sync", csp_set_timesync_command);
    add_command(&manager_script, "time_sync_broadcast", csp_timesync_broadcast_command);
    add_command(&manager_script, "get_time_sync", csp_get_timesync_command);
    add_command(&manager_script, "help_command_time_sync", help_command_time_sync);

    add_command(&manager_script, "add_reg", csp_add_register_command);
    add_command(&manager_script, "set_reg", csp_set_register_command);
    add_command(&manager_script, "get_reg", csp_get_register_command);
    add_command(&manager_script, "help_command_register", help_command_register);

    add_command(&manager_script, "set_address", csp_set_address_command);
    add_command(&manager_script, "get_address", csp_get_address_command);
    add_command(&manager_script, "reset_address", csp_reset_address_command);
    add_command(&manager_script, "set_rtable", csp_set_rtable_command);
    add_command(&manager_script, "get_rtable", csp_get_rtable_command);
    add_command(&manager_script, "reset_rtable", csp_reset_rtable_command);
    add_command(&manager_script, "help_command_settings", help_command_routing);
}

void load_script(const char *filename)
{
        if (filename == NULL)
    {
        printf("Usage: run_script <filename.txt>\n");
        return;
    }
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        printf("Error: Could not open file %s\n", filename);
        return;
    }

    char line[256];
    while (fgets(line, sizeof(line), file))
    {
        line[strcspn(line, "\n")] = '\0';

        char *command = strtok(line, " ");
        char *args = strtok(NULL, "");

        if (command)
        {
            execute_command_script(&manager_script, command, args);
        }
    }

    fclose(file);
}


volatile int running = 1; 
CSP_DEFINE_TASK(input_task) {
    char input[256];
    while (running) {
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = '\0';
        if (strcmp(input, "stop") == 0) {
            running = 0;
        }
    }
    csp_thread_exit();
    return CSP_TASK_RETURN;
}

void load_script_loop(const char *filename) {
    if (filename == NULL) {
        printf("Usage: run_script <filename.txt>\n");
        return;
    }

    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error: Could not open file %s\n", filename);
        return;
    }

    char line[256];
    csp_thread_handle_t input_thread_handle;

    if (csp_thread_create(input_task, "InputTask", 1024, NULL, 1, &input_thread_handle) != 0) {
        printf("Error: Could not create input thread.\n");
        fclose(file);
        return;
    }

    while (running) {
        if (fgets(line, sizeof(line), file)) {
            line[strcspn(line, "\n")] = '\0';

            char *command = strtok(line, " ");
            char *args = strtok(NULL, "");

            if (command) {
                printf("Running command: %s %s\n", command, args ? args : "");
                execute_command_script(&manager_script, command, args);
            }
        } else {
            rewind(file);
        }
    }

    fclose(file);

    while (running) {
        csp_sleep_ms(100);
    }
    running =1 ;
    
}



void save_script_command(const char *args)
{
    char filename[16], command[16], parameters[256];

    if (sscanf(args, "%15s %15s", filename, command) == 2)
    {

        const char *remaining_args = strchr(args + strlen(filename) + 1, ' ');
        if (remaining_args != NULL)
        {
            remaining_args++;
            strncpy(parameters, remaining_args, sizeof(parameters) - 1);
            parameters[sizeof(parameters) - 1] = '\0';

            save_script(filename, command, parameters);
        }
        else
        {
            printf("Usage: save_script <filename.txt> <command> <parameters>\n");
        }
    }
    else
    {
        printf("Usage: save_script <filename.txt> <command> <parameters>\n");
    }
}

void save_script(const char *filename, const char *command, const char *args)
{
    FILE *file = fopen(filename, "a"); // Open file in append mode
    if (file == NULL)
    {
        printf("Error: Could not open file %s for writing\n", filename);
        return;
    }

    fprintf(file, "%s %s\n", command, args); // Write command and args to file
    fclose(file);
    printf("Command saved to script: %s %s\n", command, args);
}

void clean_script(const char *filename)
{
    if (filename == NULL)
    {
        printf("Usage: clean_script <filename.txt>\n");
        return;
    }
    FILE *file = fopen(filename, "w"); // Open file in write mode to clear its content
    if (file == NULL)
    {
        printf("Error: Could not open file %s for cleaning\n", filename);
        return;
    }

    // Close the file immediately to reset its content
    fclose(file);
    printf("Script %s has been cleaned.\n", filename);
}

void delete_script(const char *filename)
{

    if (filename == NULL)
    {
        printf("Usage: delete_script <filename.txt>\n");
        return;
    }
    if (remove(filename) == 0)
    {
        printf("Script %s has been deleted.\n", filename);
    }
    else
    {
        printf("Error: Could not delete script %s\n", filename);
    }
}

// linux
void open_with_nano(const char *filename)
{
    if (filename == NULL)
    {
        printf("Usage: open_nano <filename.txt>\n");
        return;
    }
    char command[256];
    snprintf(command, sizeof(command), "nano %s", filename);

    int result = system(command);

    if (result == -1)
    {
        printf("Error: Could not execute nano\n");
    }
}

// windows
void open_with_notepad(const char *filename)
{
    if (filename == NULL)
    {
        printf("Usage: open_notepad <filename.txt>\n");
        return;
    }

    char command[256];
    snprintf(command, sizeof(command), "notepad %s", filename);

    int result = system(command);

    if (result == -1)
    {
        printf("Error: Could not execute notepad\n");
    }
}

