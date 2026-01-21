#ifndef SCRIPT_MANAGEMENT_H
#define SCRIPT_MANAGEMENT_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <include/csp/csp.h>
#include <include/csp/arch/csp_thread.h>
#include "cli_rtable.h"
#include "cli_csp.h"
// #include "cli_func_adcs.c"
// #include "cli_func_csp.c"
// #include "cli_func_ssdv.c"
// #include "cli_func_time_sync.c"
// #include "cli_func_register.c"
#include "print_list.h"


typedef struct {
    const char *command;
    void (*execute)(const char *args);
} Command_script;

typedef struct {
    Command_script *commands;  
    int count;        
} CommandManager;

extern CommandManager manager_script;

void add_command(CommandManager *manager, const char *name, void (*execute)(const char *args));
void remove_command(CommandManager *manager, const char *name);
void list_commands(CommandManager *manager);
void list_commands_print(const char *args);
void execute_command_script(CommandManager *manager, const char *command, const char *args);
void initiate_command_script();

void load_script(const char *filename);
void load_script_loop(const char *filename);
void save_script(const char *filename, const char *command, const char *args);
void save_script_command(const char *args);
void clean_script(const char *filename);
void delete_script(const char *filename);
void open_with_nano(const char *filename);
void open_with_notepad(const char *filename);


#endif
