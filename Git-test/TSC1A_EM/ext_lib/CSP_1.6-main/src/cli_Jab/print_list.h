#ifndef PRINT_LIST_H
#define PRINT_LIST_H



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>


void display_help(const char *args);

void help_command(const char *args);
void help_command_csp(const char *args);
void help_command_adcs(const char *args);
void help_command_dhu (const char *args);
void help_command_chicken(const char *args);
void help_command_ssdv(const char *args);
void help_command_register(const char *args);
void help_command_time_sync(const char *args);
void help_command_routing(const char *args);
void help_command_script(const char *args);
void print_details(const char *args);
void print_ascii_art(const char *args);

void print_colored_char(char c, int color_code);
void display_initialising_with_color(const char *text);
void print_colored_char1(char c, int color_code, int is_bold);
void display_initialising_with_effect(const char *text);
void print_scrolling_text(const char *text);
void print_blue_typing_effect(const char *text);
#endif